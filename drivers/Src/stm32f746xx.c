/*
 * stm32f746xx.c
 *
 *  Created on: Aug 5, 2025
 *      Author: s0953
 */


#include "stm32f746xx.h"
uint32_t SystemCoreClock = 16000000;
void delay_ms(uint32_t ms)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = ms * (SystemCoreClock / 1000); // SystemCoreClock 要自己定義

    while ((DWT->CYCCNT - start) < cycles);
}

void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000);

    while ((DWT->CYCCNT - start) < cycles);
}
void NVIC_SetPriorityGrouping_Manual(uint32_t prigroup) {
    uint32_t reg;

    reg = SCB_AIRCR;
    reg &= ~(SCB_AIRCR_VECTKEY_MASK | SCB_AIRCR_PRIGROUP_MASK); // 清除舊值
    reg |= SCB_AIRCR_VECTKEY | (prigroup << 8);
    SCB_AIRCR = reg;
}