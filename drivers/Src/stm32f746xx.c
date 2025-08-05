/*
 * stm32f746xx.c
 *
 *  Created on: Aug 5, 2025
 *      Author: s0953
 */


#include "stm32f746xx.h"

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
