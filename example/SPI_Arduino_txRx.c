/*
 * SPI_arduino.c
 *
 *  Created on: Jun 26, 2025
 *      Author: s0953
 */


#include "stm32f746xx.h"
#include <string.h>
/*
SPI2
(Arduino pin )
AF5
PI0 ---> NSS
PI1 ---> SCK

AF5
PB14 --> MISO
PB15 --> MOSI

*/

/*
#define COMMAND_LED_CTRL      	0x50
#define COMMAND_SENSOR_READ   	0x51
#define COMMAND_LED_READ      	0x52
#define COMMAND_PRINT         	0x53
#define COMMAND_ID_READ       	0x54

#define LED_ON				  	1
#define LED_OFF					0		

//arduino
#define ALALOG_PIN0				0
#define ALALOG_PIN1				1
#define ALALOG_PIN2				2
#define ALALOG_PIN3				3
#define ALALOG_PIN4				4

#define LED_PIN					9



static void SPI2_GPIO_Inits(void){
	GPIO_Handle_t SPIPinsB;
	SPIPinsB.pGPIOx = GPIOB;
	SPIPinsB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPinsB.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPinsB.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPinsB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPinsB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//MISO
	SPIPinsB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPinsB);
	//MOSI
	SPIPinsB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPinsB);

	GPIO_Handle_t SPIPinsI;
	SPIPinsI.pGPIOx = GPIOI;
	SPIPinsI.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPinsI.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPinsI.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPinsI.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPinsI.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//NSS
	SPIPinsI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIO_Init(&SPIPinsI);
	//SCK
	SPIPinsI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	GPIO_Init(&SPIPinsI);
}

int interrupt_flag = 0;

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		//ack
		return 1;
	}else
	{
		//nack
		return 0;
	}

}
int main(){


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



	SPI2_GPIO_Inits();

	//SPI2 init
	SPI_Handle_t SPI2handle;
	SPI2handle.pSPIx = SPI2I2S2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; //8MHz
	SPI2handle.SPIConfig.SPI_DS = SPI_DS_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;
	SPI_Init(&SPI2handle);

	//SPI_SSIConfig(&SPI2handle,ENABLE);

	SPI_SSOEConfig(&SPI2handle, ENABLE);


	

	while(1){

		
		if(interrupt_flag)
		{
			

			// 1. CMD_LED_CTRL <pin no(1)> <value(1)>
			uint8_t commandcode = COMMAND_LED_CTRL;
			uint8_t ackbyte;
			uint8_t args[2];
			uint8_t dummy_read;
			uint8_t dummy_byte = 0xff;
			SPI_PeripheralControl(&SPI2handle, ENABLE);
			
			SPI_SendData(&SPI2handle, &commandcode, 1);
			//do dummy read to clear off the RXNE
			//每一次送資料給對方 因為SPI的特性，對方也一定會有東西送過來，但不一定是有意義的
			//因此我們要去讀資料 把RXNE清掉 把DR清空
			SPI_ReceiveData(&SPI2handle,&dummy_read,1);


			// 2.send dome dummy bits to fetch the response form the slave
			//(slave的ack已經預備在arduino的tx shift reg,我們需要送dummy byte把對方的ack 移動出來)
			SPI_SendData(&SPI2handle, &dummy_byte, 1);
			//read the ack byte received
			SPI_ReceiveData(&SPI2handle,&ackbyte,1);

			if(SPI_VerifyResponse(ackbyte))
			{
				//send arguments
				args[0] = LED_PIN;
				args[1] = LED_ON;
				SPI_SendData(&SPI2handle, args, 2);
				//read the ack byte received
				SPI_ReceiveData(&SPI2handle,&ackbyte,1);

			}


			while(SPI_GetFlagStatus(&SPI2handle, SPI_BSY_FLAG));
			interrupt_flag = 0;
			SPI_PeripheralControl(&SPI2handle, DISABLE);
		}
	}
	return 0;
}


void EXTI15_10_IRQHandler(void){
	GPIO_IRQHandling(GPIO_PIN_NO_11);// clear the pending register
	interrupt_flag = 1;
}
*/
