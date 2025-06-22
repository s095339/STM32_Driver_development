/*
 * LED_toggle.c
 *
 *  Created on: Jun 20, 2025
 *      Author: s0953
 */

#include "stm32f746xx.h"

static void delay(void)
{
	for(uint32_t i = 0; i<500000; i++);
}
int main_(void)
{
	// configurate GPIO
	GPIO_Handle_t gpio_led;
	gpio_led.pGPIOx=GPIOI;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpio_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOI, ENABLE);
	GPIO_Init(&gpio_led);

	GPIO_Handle_t btn;
	btn.pGPIOx = GPIOI;//PI11
	btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpio_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&btn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOI, GPIO_PIN_NO_11) == 1)
			GPIO_ToggleOutputPin(GPIOI, GPIO_PIN_NO_1);
		delay();

	}
	return 0;
}
