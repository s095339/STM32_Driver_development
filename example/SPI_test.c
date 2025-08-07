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

int main_SPI(){

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
	SPI_PeripheralControl(&SPI2handle, ENABLE);

	SPI_SendData(&SPI2handle, (uint8_t*)user_data, strlen(user_data));

	while(1);
	return 0;
}
