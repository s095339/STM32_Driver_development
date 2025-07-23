/*
 * i2c_blocking_ControllerReceive.c
 *
 *  Created on: Jul 22, 2025
 *      Author: s0953
 */
#include "stm32f746xx.h"
#include <stdio.h>


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





#define READ_COMMAND_LENGTH 0X51
#define READ_COMMAND_DATA   0x52

I2C_Handle_t I2C1_Handle = {
		.pI2Cx = I2C1,
		.I2C_Config = {
			.I2C_AddrMode 	= I2C_ADDRMODE_7BIT,
			.I2C_ClkSrc 	= I2C_CLKSRC_PCLK1,
			.I2C_SCLSpeed   = I2C_SCL_SPEED_SM100k,
			.I2C_DeviceAddress = 0x68
		}
	};


int main(void)
{

	


	// btn interrupt

	// GPIO for I2C1
	I2C1_GPIO_Inits();
	// I2C Init
	I2C_Init(&I2C1_Handle);
	
	I2C_IRQITConfig(IRQ_NO_I2C1_EV, ENABLE);


	for(;;)
	{
		
	}
	return 0;
}

char Tx_buf[] = "Hello this is STM32";
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	static uint8_t command_code;
	static uint8_t Cnt = 0;
	if(AppEv == I2C_EV_TARGET_TRAN)
	{

	}
	if(AppEv == I2C_EV_TARGET_RCV)
	{
		
	}
	if(AppEv == I2C_EV_TARGET_STOP)
	{
		Cnt = 0;
	}
	if(AppEv == I2C_EV_DATA_REQ)
	{
		if(command_code == 0x51)
		{
			//send the length information to the master
			I2C_SlaveSendData(pI2CHandle->pI2Cx,strlen((char*)Tx_buf));
		}else if (command_code == 0x52)
		{
			//Send the contents of Tx_buf
			I2C_SlaveSendData(pI2CHandle->pI2Cx,(uint8_t)Tx_buf[Cnt++]);

		}
	}
	if(AppEv == I2C_EV_DATA_RCV)
	{
		command_code = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
		

	}
}
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1_Handle);
}
