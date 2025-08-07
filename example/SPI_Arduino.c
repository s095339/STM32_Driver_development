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

static  int interrupt_flag = 0;
int main__spi(){


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



	char user_data[] = "Hello Arduino";

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
	
	SPI_SSOEConfig(SPI2I2S2, ENABLE);
	
	SPI_PeripheralControl(SPI2I2S2, ENABLE);

	//first send length info
	uint8_t dataLen = strlen(user_data);
	SPI_SendData(SPI2I2S2, &dataLen,1);
	SPI_SendData(SPI2I2S2, (uint8_t*)user_data, dataLen);

	//confirm SPI is not busy
	while(SPI_GetFlagStatus(SPI2I2S2, SPI_BSY_FLAG));
	interrupt_flag = 0;
	SPI_PeripheralControl(SPI2I2S2, DISABLE);
	

	while(1){
		
		if(interrupt_flag)
		{
			SPI_PeripheralControl(SPI2I2S2, ENABLE);

			//first send length info
			uint8_t dataLen = strlen(user_data);
			SPI_SendData(SPI2I2S2, &dataLen,1);
			SPI_SendData(SPI2I2S2, (uint8_t*)user_data, dataLen);

			//confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2I2S2, SPI_BSY_FLAG));
			interrupt_flag = 0;
			SPI_PeripheralControl(SPI2I2S2, DISABLE);
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
