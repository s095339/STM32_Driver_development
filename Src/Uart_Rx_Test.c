/*
 * Uart_Tx_Test.c
 *
 *  Created on: Jul 25, 2025
 *      Author: s0953
 */


#include "stm32f746xx.h"
#include <stdio.h>

static void delay()
{
	for(uint32_t i=0; i<160000; i++)
	{

	}
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

static void GPIO_Interrupt_Inits(void){
	GPIO_Handle_t btn;
	memset(&btn, 0, sizeof(btn));
	btn.pGPIOx = GPIOI;//PI11
	btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	btn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&btn);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_RRI15);//optional 有多個interrupt時才有必要去設定這個
	//開啟GPIOI pin number = 11的interrupt 那他將會是EXTI11，Interrupt number:40
	GPIO_IRQITConfig(IRQ_NO_EXTI15_10, ENABLE);
}

static void I2C1_GPIO_Inits(void){
	GPIO_Handle_t I2CPinsB;
	I2CPinsB.pGPIOx = GPIOB;
	I2CPinsB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPinsB.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPinsB.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; //i2c需要opendrain
	I2CPinsB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPinsB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPinsB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GPIO_Init(&I2CPinsB);
	//SDA
	I2CPinsB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&I2CPinsB);
}



static uint8_t gpio_intr = 0;
static uint8_t intr = 0;
UART_Handle_t usart6;

volatile char RxBuff[32];
volatile char TxBuff[] = "Greeting from STM32\n";

char *r,*t;
uint8_t uart_rx_cmpty_flag = 0;
int main()
{

	GPIO_Interrupt_Inits();
	usart6.pUARTx = USART6;
	usart6.UART_Config.UART_Baud = UART_STD_BAUD_9600;
	usart6.UART_Config.UART_Mode = UART_MODE_TXRX;
	usart6.UART_Config.UART_NoOfStopBits = UART_STOPBITS_1;
	usart6.UART_Config.UART_ParityControl = UART_PARITY_DISABLE;
	usart6.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	usart6.UART_Config.UART_HWFlowControl = UART_HW_FLOW_CTRL_NONE;
	usart6.RxState = UART_READY;
	usart6.TxState = UART_READY;
	usart6.RxLenUnknow = UART_RXLEN_KNOWN;

	UART_Init(&usart6);
	UART_IRQInterruptConfig(IRQ_NO_USART6, ENABLE);
	UART_IRQPriorityConfig(IRQ_NO_USART6, 14);
	UART_GPIO_Inits();

	I2C_Handle_t I2C_Handle = {
		.pI2Cx = I2C1,
		.I2C_Config = {
			.I2C_AddrMode 	= I2C_ADDRMODE_7BIT,
			.I2C_ClkSrc 	= I2C_CLKSRC_PCLK1,
			.I2C_SCLSpeed   = I2C_SCL_SPEED_SM100k,
			.I2C_DeviceAddress = 0x61
		}
	};
	// GPIO for I2C1
	I2C1_GPIO_Inits();
	I2C_Init(&I2C_Handle);

	//I2C_ControllerSendData(&I2C_Handle,(uint8_t *)0x45,1, 0x68, ENABLE);
	//while(UART_ReceiveDataIT(&usart6, (uint8_t*)RxBuff, strlen(TxBuff)) != UART_READY);
	//while(UART_SendDataIT(&usart6, (uint8_t*)TxBuff, strlen(TxBuff)) != UART_READY);

	while(1)
	{
		
		if(uart_rx_cmpty_flag)
		{
			uart_rx_cmpty_flag = 0;
			I2C_ControllerSendData(&I2C_Handle,(uint8_t *)RxBuff,strlen(RxBuff), 0x68, ENABLE);
		}

		if(gpio_intr)
		{
			gpio_intr = 0;
			
			while(UART_ReceiveDataIT(&usart6, (uint8_t*)RxBuff, strlen(TxBuff)) != UART_READY);

			UART_SendDataIT(&usart6, (uint8_t*)TxBuff, strlen(TxBuff));
		}
		
	}

	return 0;
}




void USART6_IRQHandler()
{
	UART_IRQHandling(&usart6);
}

void UART_ApplicationEventCallback(UART_Handle_t *pUARTHandle,uint8_t AppEv)
{
	//static uint32_t a = 0;
	//static uint32_t b = 0;
	if(AppEv == UART_EV_RX_COMPLT)
	{

		uart_rx_cmpty_flag = 1;
		//a=strlen(RxBuff);
	}
	if(AppEv == UART_EV_TX_COMPLT)
	{
		//b=strlen(TxBuff);
	}
}
void EXTI15_10_IRQHandler(void){
	GPIO_IRQHandling(GPIO_PIN_NO_11);// clear the pending register
	gpio_intr = 1;
}
