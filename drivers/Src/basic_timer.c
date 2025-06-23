/*
 * basic_timer.c
 *
 *  Created on: Jun 23, 2025
 *      Author: s0953
 */


#include "basic_timer.h"


void BasicTimer_Init(BasicTimer_Config_t * pBTimCongif)
{
    if(pBTimCongif->TIMERx == TIM6) TIMER6_PLCK_EN(); 
    if(pBTimCongif->TIMERx == TIM7) TIMER7_PLCK_EN(); 
    pBTimCongif->TIMERx->CR1.UDIS |= pBTimCongif->TIMER_UDIS;
    pBTimCongif->TIMERx->CR1.URS |= pBTimCongif->TIMER_URS;
    pBTimCongif->TIMERx->CR1.OPM |= pBTimCongif->TIMER_OPM;
    pBTimCongif->TIMERx->CR1.ARPE |= pBTimCongif->TIMER_APRE;
    pBTimCongif->TIMERx->CR1.UIFREMA |= pBTimCongif->TIMER_UIFREMA;
    
    pBTimCongif->TIMERx->CR2.MMS |= pBTimCongif->TIMER_MMS;

    pBTimCongif->TIMERx->DIER.UIE |= pBTimCongif->TIMER_UIE;
    pBTimCongif->TIMERx->DIER.UDE |= pBTimCongif->TIMER_UDE;
    pBTimCongif->TIMERx->SR |= pBTimCongif->TIMER_SR;
    pBTimCongif->TIMERx->EGR |= pBTimCongif->TIMER_EGR;
    
    pBTimCongif->TIMERx->PSC = pBTimCongif->TIMER_PSC;
    pBTimCongif->TIMERx->ARR = pBTimCongif->TIMER_ARR;
}
void BasicTimer_start(BasicTimer_Config_t* tim){
    tim->TIMERx->CR1.CEN |= 1;
}
void BasicTimer_DeInit(BasicTimer_Config_t* tim)
{
    
    RCC->APB1RSTR |= (1<<4);
}

void BasicTimerInterrupt_Handling(BasicTimer_RegDef_t* tim)
{
    tim->SR &= ~(1<<0);//clear the UIF


}

void BasicTimer_ITConfig(BasicTimer_Config_t* tim, uint8_t EnorDi)
{   uint8_t IRQNumber;
    if(tim->TIMERx == TIM6) IRQNumber = 54;
    if(tim->TIMERx == TIM7) IRQNumber = 55;
    Global_IRQITConfig(IRQNumber, EnorDi);
}
void BasicTimer_IRQPriorityConfig(BasicTimer_Config_t* tim, uint8_t IRQPriority)
{
    uint8_t IRQNumber;
    if(tim->TIMERx == TIM6) IRQNumber = 54;
    if(tim->TIMERx == TIM7) IRQNumber = 55;
    Global_IRQPriorityConfig(IRQNumber, IRQPriority);
}
