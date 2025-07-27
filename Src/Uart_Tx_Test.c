/*
 * Uart_Tx_Test.c
 *
 *  Created on: Jul 25, 2025
 *      Author: s0953
 */


#include "stm32f746xx.h"
#include <stdio.h>

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


static uint8_t interrupt_flag = 0;
UART_Handle_t usart6;
char buff[] = "Greeting from USART of STM32";

int main()
{
	GPIO_Interrupt_Inits();

	usart6.pUARTx = USART6;
	usart6.UART_Config.UART_Baud = UART_STD_BAUD_115200;
	usart6.UART_Config.UART_Mode = UART_MODE_TXRX;
	usart6.UART_Config.UART_NoOfStopBits = UART_STOPBITS_1;
	usart6.UART_Config.UART_ParityControl = UART_PARITY_DISABLE;
	usart6.UART_Config.UART_WordLength = UART_WORDLEN_8BITS;
	usart6.UART_Config.UART_HWFlowControl = UART_HW_FLOW_CTRL_NONE;

	UART_Init(&usart6);
	UART_GPIO_Inits();

	while(1)
	{
		if(interrupt_flag)
		{
			interrupt_flag = 0;
			UART_SendData(&usart6, (uint8_t*)buff, strlen(buff));
			printf("msg sent");
		}
	}

	return 0;
}




void EXTI15_10_IRQHandler(void){
	GPIO_IRQHandling(GPIO_PIN_NO_11);// clear the pending register
	interrupt_flag = 1;
}
