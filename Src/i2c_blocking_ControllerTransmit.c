/*
 * i2c_blocking_ControllerTransmit.c
 *
 *  Created on: Jul 21, 2025
 *      Author: s0953
 */

#include "stm32f746xx.h"
void delay(void)
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

int main_i2cmastertx(void)
{

	I2C_Handle_t I2C_Handle = {
		.pI2Cx = I2C1,
		.I2C_Config = {
			.I2C_AddrMode 	= I2C_ADDRMODE_7BIT,
			.I2C_ClkSrc 	= I2C_CLKSRC_PCLK1,
			.I2C_SCLSpeed   = I2C_SCL_SPEED_SM100k,
			.I2C_DeviceAddress = 0x61
		}
	};


	// btn interrupt
	GPIO_Interrupt_Inits();
	// GPIO for I2C1
	I2C1_GPIO_Inits();
	// I2C Init
	I2C_Init(&I2C_Handle);

	uint8_t Len = 10;
	char pTxBuff[] = "Hello I2C";
	for(;;)
	{
		if(interrupt_flag)
		{
			interrupt_flag = 0;
			I2C_ControllerSendData(&I2C_Handle,(uint8_t *)pTxBuff,strlen(pTxBuff), 0x68, ENABLE);
		}
	}
	return 0;
}


/*
void EXTI15_10_IRQHandler(void){
	GPIO_IRQHandling(GPIO_PIN_NO_11);// clear the pending register
	interrupt_flag = 1;
}
*/