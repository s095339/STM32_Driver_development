/*
 * i2c_blocking_ControllerReceive.c
 *
 *  Created on: Jul 22, 2025
 *      Author: s0953
 */
#include "stm32f746xx.h"
#include <stdio.h>
static void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
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

/* 
 * I2C1
 * SCL:	PB8
 * SDA:	PB9
 */

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



static uint8_t interrupt_flag = 0;

#define READ_COMMAND_LENGTH 0X51
#define READ_COMMAND_DATA   0x52

I2C_Handle_t I2C_Handle = {
		.pI2Cx = I2C1,
		.I2C_Config = {
			.I2C_AddrMode 	= I2C_ADDRMODE_7BIT,
			.I2C_ClkSrc 	= I2C_CLKSRC_PCLK1,
			.I2C_SCLSpeed   = I2C_SCL_SPEED_SM100k,
			.I2C_DeviceAddress = 0x61
		}
	};

int main(void)
{

	


	// btn interrupt
	GPIO_Interrupt_Inits();
	// GPIO for I2C1
	I2C1_GPIO_Inits();
	// I2C Init
	I2C_Init(&I2C_Handle);
	
	I2C_IRQITConfig(IRQ_NO_I2C1_EV, ENABLE);




	char pRxBuff[255];

    uint8_t commandcode;
	uint8_t len;
	for(;;)
	{
		if(interrupt_flag)
		{
			interrupt_flag = 0;
			commandcode = 0x51;


			while(I2C_ControllerSendDataIT(&I2C_Handle,&commandcode,1,0x68,ENABLE) != I2C_READY);

			while(I2C_ControllerReceiveDataIT(&I2C_Handle,&len,1,0x68,ENABLE)!= I2C_READY);



			commandcode = 0x52;
			while(I2C_ControllerSendDataIT(&I2C_Handle,&commandcode,1,0x68,ENABLE) != I2C_READY);


			while(I2C_ControllerReceiveDataIT(&I2C_Handle,(uint8_t*)pRxBuff,len,0x68,ENABLE)!= I2C_READY);
		}
	}
	return 0;
}


void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	
	if(AppEv == I2C_EV_TX_CMPLT)
	{

	}
	if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("%s", pI2CHandle->pRxBuffer);
	}
}
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C_Handle);
}
void EXTI15_10_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_11);// clear the pending register
	interrupt_flag = 1;
}

