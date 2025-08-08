/*
 * main.c
 *
 *  Created on: Aug 4, 2025
 *      Author: s0953
 */


#include "main.h"
//因為有浮點樹運算所以需要打開FPU



//Peripheral Initialization===================
UART_Handle_t usart6;
I2C_Handle_t hi2c1 = {
		.pI2Cx = I2C1,
		.I2C_Config = {
			.I2C_AddrMode 	= I2C_ADDRMODE_7BIT,
			.I2C_ClkSrc 	= I2C_CLKSRC_PCLK1,
			.I2C_SCLSpeed   = I2C_SCL_SPEED_SM100k,
			.I2C_DeviceAddress = 0x61,
		},
		.TxRxState = I2C_READY
	};
static void I2C1_GPIO_Inits(void);
static void UART_GPIO_Inits(void);
//================FreeRTOS================//
//Task
xTaskHandle handle_cmd_task;
xTaskHandle handle_menu_task;
xTaskHandle handle_uart_print_task;
xTaskHandle handle_i2c_task;
xTaskHandle handle_th_task;

void cmd_task(void * parameters);
void menu_task(void * parameters);
void uart_print_task(void * parameters);
void i2c_task(void * parameters);
void th_task(void * parameters);
//queue
QueueHandle_t q_uartrx;
QueueHandle_t q_uarttx;
QueueHandle_t q_i2crx;
QueueHandle_t q_i2ctx;

//for uart
volatile uint8_t user_data;
volatile uint8_t dummy;

OLED_state oled_state;

int main(void)
{

	//=================//
	// Processor  Init //
	//=================//
	// Enable DWT cyccnt
	CoreDebug->DEMCR |= (1 << 24);
	DWT_LAR = 0xC5ACCE55;
	DWT->CYCCNT = 0;
	DWT->CTRL |= 1;

	CPACR |= (0xF << 20);//浮點樹運算開啟

	NVIC_SetPriorityGrouping_Manual(0);//priority grouping  設為4(freeRTOS要求)
	//=================//
	// Peripheral Init //
	//=================//
	//I2C Init
	hi2c1.TxRxState = I2C_READY;
	I2C1_GPIO_Inits();
	I2C_Init(&hi2c1);
	I2C_IRQPriorityConfig(IRQ_NO_I2C1_EV, 9);
	I2C_IRQITConfig(IRQ_NO_I2C1_EV, ENABLE);
	//Uart init
	usart6.pUARTx = USART6;
	usart6.UART_Config.UART_Baud = UART_STD_BAUD_9600;
	usart6.UART_Config.UART_Mode = UART_MODE_TXRX;
	usart6.UART_Config.UART_NoOfStopBits = UART_STOPBITS_1;
	usart6.UART_Config.UART_ParityControl = UART_PARITY_DISABLE;
	usart6.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	usart6.UART_Config.UART_HWFlowControl = UART_HW_FLOW_CTRL_NONE;
	usart6.RxState = UART_READY;
	usart6.TxState = UART_READY;
	UART_Init(&usart6);
	UART_IRQPriorityConfig(IRQ_NO_USART6, 10);
	UART_IRQInterruptConfig(IRQ_NO_USART6, ENABLE);
	
	UART_GPIO_Inits();
	//=================//
	// BSP        Init //
	//=================//

	//Temperature & Humid Rate
	SHT2x_Init(&hi2c1);
	SHT2x_SetResolution(RES_14_12);

	//LCD
	/* Initialize */
	//  HD44780_Init(2);
	  /* Clear buffer */
	//  HD44780_Clear();
	  /* Hide characters */
	//  HD44780_NoDisplay();
	//  HD44780_Cursor();
	//  HD44780_SetCursor(0,0);
	//  HD44780_PrintStr("HELLO STM32!!!");
	//  HD44780_PrintSpecialChar(0);
	  /* Show characters */
	//  HD44780_Display();
	  /* Move position */
	//  HD44780_SetCursor(0, 1);
	//  HD44780_PrintStr("BYE STM32!!!");
	//  HD44780_PrintSpecialChar(1);
	  /* Blink cursor */
	//  HD44780_Blink();

	//OLED
	
	SSD1306_Init();
	oled_state = oIdle;
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Temp&Humid", &Font_11x18, 0);
	SSD1306_GotoXY(0, 30);
	SSD1306_Puts("Sensor", &Font_11x18, 1);
	SSD1306_UpdateScreen();

	delay_ms(3000);
	SSD1306_Clear();
	//SSD1306_DrawBitmap(0, 0, img, 128, 64, 1);
	
	//HD44780_Clear();

	//Free RTOS ================================//
	BaseType_t status;

	status = xTaskCreate(menu_task, "menu_task", 250, NULL, 2, &handle_menu_task);
	configASSERT(status == pdPASS);
	status = xTaskCreate(cmd_task, "cmd_task", 250, NULL, 2, &handle_cmd_task);
	configASSERT(status == pdPASS);
	status = xTaskCreate(uart_print_task, "uart_task", 250, NULL, 2, &handle_uart_print_task);
	configASSERT(status == pdPASS);
	status = xTaskCreate(i2c_task, "i2c_task", 250, NULL, 2, &handle_i2c_task);
	configASSERT(status == pdPASS);
	//status = xTaskCreate(th_task, "th_task", 250, NULL, 2, &handle_th_task);
	//configASSERT(status == pdPASS);

	q_uartrx = xQueueCreate(10, sizeof(char));
  	configASSERT(q_uartrx != NULL);
	q_uarttx = xQueueCreate(10, sizeof(size_t));
	configASSERT(q_uarttx != NULL);
	q_i2crx = xQueueCreate(10, sizeof(size_t));
  	configASSERT(q_i2crx != NULL);
	q_i2ctx = xQueueCreate(10, sizeof(size_t));
 	configASSERT(q_i2ctx != NULL);

	while(UART_ReceiveDataIT(&usart6, &user_data, 1) != UART_READY);


	vTaskStartScheduler();
	//==========================================//
	for(;;)
	{
		unsigned char buffer[100] = { 0 };
		/* Gets current temperature & relative humidity. */
		float cel = SHT2x_GetTemperature(0);
		/* Converts temperature to degrees Fahrenheit and Kelvin */
		float fah = SHT2x_CelsiusToFahrenheit(cel);
		float kel = SHT2x_CelsiusToKelvin(cel);
		float rh = SHT2x_GetRelativeHumidity(1);
		/* May show warning below. Ignore and proceed. */
		sprintf(buffer,
				"%d.%dºC, %d.%dºF, %d.%d K, %d.%d%% RH\n",
				SHT2x_GetInteger(cel), SHT2x_GetDecimal(cel, 1),
				SHT2x_GetInteger(fah), SHT2x_GetDecimal(fah, 1),
				SHT2x_GetInteger(kel), SHT2x_GetDecimal(kel, 1),
				SHT2x_GetInteger(rh), SHT2x_GetDecimal(rh, 1));
		//HAL_UART_Transmit(&huart1, buffer, strlen(buffer), 1000);
		UART_SendData(&usart6, (uint8_t*)buffer, strlen(buffer));

		sprintf(buffer,
						"%d.%d C",
						SHT2x_GetInteger(cel), SHT2x_GetDecimal(cel, 1),
						SHT2x_GetInteger(fah), SHT2x_GetDecimal(fah, 1));

		//LCD
		//HD44780_SetCursor(0,0);
		//HD44780_PrintStr(buffer);
		//HD44780_PrintSpecialChar(1);
		//OLED
		SSD1306_GotoXY(0, 0);
	   SSD1306_Puts(buffer, &Font_11x18, 1);



		sprintf(buffer,
								"%d.%d%% RH",
								SHT2x_GetInteger(kel), SHT2x_GetDecimal(kel, 1),
								SHT2x_GetInteger(rh), SHT2x_GetDecimal(rh, 1));
		//LCD
		//HD44780_Cursor();
		//HD44780_SetCursor(0,1);
		//HD44780_PrintStr(buffer);
		//HD44780_PrintSpecialChar(0);
		//OLED
		SSD1306_GotoXY(0, 30);
		   SSD1306_Puts(buffer, &Font_11x18, 1);
		   SSD1306_UpdateScreen();

		//delay();
		//ssd1306_TestAll();
		//delay();

	}
	return 0;
}
void USART6_IRQHandler()
{
	UART_IRQHandling(&usart6);
}
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&hi2c1);
}


static void I2C1_GPIO_Inits(void){
	GPIO_Handle_t I2CPinsB;
	I2CPinsB.pGPIOx = GPIOB;
	I2CPinsB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPinsB.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPinsB.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; //i2c需要opendrain
	I2CPinsB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	I2CPinsB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPinsB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GPIO_Init(&I2CPinsB);
	//SDA
	I2CPinsB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&I2CPinsB);
}
static void UART_GPIO_Inits(void){
	GPIO_Handle_t I2CPinsC;
	I2CPinsC.pGPIOx = GPIOC;
	I2CPinsC.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPinsC.GPIO_PinConfig.GPIO_PinAltFunMode = 8;
	I2CPinsC.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; //i2c需要opendrain
	I2CPinsC.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPinsC.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//Tx
	I2CPinsC.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPinsC);
	//Rx
	I2CPinsC.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPinsC);
}


void UART_ApplicationEventCallback(UART_Handle_t *pUARTHandle,uint8_t AppEv)
{
	//static uint32_t a = 0;
	//static uint32_t b = 0;
	if(AppEv == UART_EV_RX_COMPLT)
	{
		if(xQueueSendFromISR(q_uartrx, (void *)&user_data, NULL) == pdTRUE)
		{
			
		}

		if((char)user_data == '0')
		{
			xTaskNotifyFromISR(handle_cmd_task, 0, eNoAction,NULL);
		}
		UART_ReceiveDataIT(&usart6, &user_data, 1);
		//a=strlen(RxBuff);
	}
	if(AppEv == UART_EV_TX_COMPLT)
	{
	}
}

uint16_t data = 0; 
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv){
	if(AppEv == I2C_EV_TX_CMPLT)
	{

	}
	if(AppEv == I2C_EV_RX_CMPLT)
	{
		//一定是收到三筆data

		//I2C rx中斷發生 i2c_rx_data有我們要的資料
		
		data = 0;
		data |= ((uint16_t)i2c_rx_data[0] << 8);
		data |= (uint16_t)i2c_rx_data[1];
		if(xQueueSendFromISR(q_i2crx, (void *)&data, NULL))
		{
			xTaskNotifyFromISR(handle_i2c_task, 0, eNoAction, NULL);
		}	
	}
}
