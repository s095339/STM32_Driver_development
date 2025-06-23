/*
 * LED_toggle.c
 *
 *  Created on: Jun 20, 2025
 *      Author: s0953
 */

#include "stm32f746xx.h"
#include <string.h>
static void delay(void)
{
	for(uint32_t i = 0; i<500000; i++);
}
int main(void)
{
	// configurate GPIO
	GPIO_Handle_t gpio_led;
	GPIO_Handle_t btn;
	memset(&gpio_led, 0, sizeof(gpio_led));//initial  the menbers of structure to 0; (<string.h>)
	

	gpio_led.pGPIOx=GPIOI;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpio_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOI, ENABLE);
	GPIO_Init(&gpio_led);


	btn.pGPIOx = GPIOI;//PI11
	btn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	btn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	btn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	btn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&btn);


	BasicTimer_Config_t tim;
	memset(&tim, 0, sizeof(tim));
	tim.TIMERx = TIM7;
	tim.TIMER_PSC = 16;
	tim.TIMER_ARR = 65535;
	tim.TIMER_UDE = 0;
	tim.TIMER_UIE = 1;
	BasicTimer_Init(&tim);
	BasicTimer_start(&tim);
	
	/**
	 * Interrupt Configuration
	 *
	 */
	//GPIOI pin number = 11的interrupt 那他將會是EXTI11，Interrupt number:40

	BasicTimer_IRQPriorityConfig(&tim, NVIC_IRQ_RRI15);//optional 有多個interrupt時才有必要去設定這個
	
	//開啟GPIOI pin number = 11的interrupt 那他將會是EXTI11，Interrupt number:40
	//GPIO_IRQITConfig(IRQ_NO_EXTI15_10, ENABLE);
	BasicTimer_ITConfig(&tim, ENABLE);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_RRI15);//optional 有多個interrupt時才有必要去設定這個
	//開啟GPIOI pin number = 11的interrupt 那他將會是EXTI11，Interrupt number:40
	GPIO_IRQITConfig(IRQ_NO_EXTI15_10, ENABLE);

	while(1)
	{
	}
	return 0;
}


void TIM7_IRQHandler(void)
{
	BasicTimerInterrupt_Handling(TIM7);
	GPIO_ToggleOutputPin(GPIOI, GPIO_PIN_NO_1);

}
void EXTI15_10_IRQHandler(void){
	GPIO_IRQHandling(GPIO_PIN_NO_11);// clear the pending register

	TIM7->CR1.CEN ^= 1;
}
