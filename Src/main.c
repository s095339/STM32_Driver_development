/*
 * main.c
 *
 *  Created on: Aug 4, 2025
 *      Author: s0953
 */

#include "stm32f746xx.h"
#include <stdio.h>
#include "sht2x.h"
#include "ssd1306.h"
#include "liquidcrystal_i2c.h"
//因為有浮點樹運算所以需要打開FPU
#define CPACR (*(volatile uint32_t*)0xE000ED88)

static void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
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
I2C_Handle_t hi2c1 = {
		.pI2Cx = I2C1,
		.I2C_Config = {
			.I2C_AddrMode 	= I2C_ADDRMODE_7BIT,
			.I2C_ClkSrc 	= I2C_CLKSRC_PCLK1,
			.I2C_SCLSpeed   = I2C_SCL_SPEED_SM100k,
			.I2C_DeviceAddress = 0x61
		}
	};

UART_Handle_t usart6;

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


int main(void)
{


	CoreDebug->DEMCR |= (1 << 24);
	DWT_LAR = 0xC5ACCE55;
	DWT->CYCCNT = 0;
	DWT->CTRL |= 1;
	

	// GPIO for I2C1
	I2C1_GPIO_Inits();
	// I2C Init
	I2C_Init(&hi2c1);
	SHT2x_Init(&hi2c1);


	SHT2x_SetResolution(RES_14_12);

	CPACR |= (0xF << 20);//浮點樹運算開啟
	//Uart init
	usart6.pUARTx = USART6;
	usart6.UART_Config.UART_Baud = UART_STD_BAUD_9600;
	usart6.UART_Config.UART_Mode = UART_MODE_TXRX;
	usart6.UART_Config.UART_NoOfStopBits = UART_STOPBITS_1;
	usart6.UART_Config.UART_ParityControl = UART_PARITY_DISABLE;
	usart6.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	usart6.UART_Config.UART_HWFlowControl = UART_HW_FLOW_CTRL_NONE;
	UART_Init(&usart6);
	UART_GPIO_Inits();



	//LCD
	/* Initialize */
	  HD44780_Init(2);

	  /* Clear buffer */
	  HD44780_Clear();

	  /* Hide characters */
	  HD44780_NoDisplay();
	  HD44780_Cursor();
	  HD44780_SetCursor(0,0);
	  HD44780_PrintStr("HELLO STM32!!!");
	  HD44780_PrintSpecialChar(0);

	  /* Show characters */
	  HD44780_Display();

	  /* Move position */
	  HD44780_SetCursor(0, 1);
	  HD44780_PrintStr("BYE STM32!!!");
	  HD44780_PrintSpecialChar(1);

	  /* Blink cursor */
	  HD44780_Blink();

	//OLED
	  SSD1306_Init();
	   SSD1306_GotoXY(0, 0);
	   SSD1306_Puts("Temp&Humid", &Font_11x18, 1);
	   SSD1306_GotoXY(0, 30);
	   SSD1306_Puts("Sensor", &Font_11x18, 1);
	   SSD1306_UpdateScreen();

	   SSD1306_ScrollLeft(0, 7);
	   delay_ms(3000);
	   SSD1306_ScrollRight(0, 7);
	   delay_ms(3000);
	   SSD1306_Stopscroll();
	   delay_ms(3000);
	   SSD1306_Clear();

	   //SSD1306_DrawBitmap(0, 0, img, 128, 64, 1);
	   SSD1306_UpdateScreen();
	   HD44780_Clear();
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
		HD44780_SetCursor(0,0);
		HD44780_PrintStr(buffer);
		HD44780_PrintSpecialChar(1);
		//OLED
		SSD1306_GotoXY(0, 0);
	   SSD1306_Puts(buffer, &Font_11x18, 1);



		sprintf(buffer,
								"%d.%d%% RH",
								SHT2x_GetInteger(kel), SHT2x_GetDecimal(kel, 1),
								SHT2x_GetInteger(rh), SHT2x_GetDecimal(rh, 1));
		//LCD
		HD44780_Cursor();
		HD44780_SetCursor(0,1);
		HD44780_PrintStr(buffer);
		HD44780_PrintSpecialChar(0);
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
