/*
 * basic_timer.c
 *
 *  Created on: Jun 23, 2025
 *      Author: s0953
 */

#ifndef INC_BASIC_TIMER_H_
#define INC_BASIC_TIMER_H_

#include "stm32f746xx.h"



//Peripheral register definition structures
struct TIMx_CR1 {
    unsigned int CEN:1;
    unsigned int UDIS:1;
    unsigned int URS:1;
    unsigned int OPM:1;
    unsigned int :3;
    unsigned int ARPE:1;
    unsigned int :3;
    unsigned int UIFREMA:1;
    unsigned int :20;
};
struct TIMx_CR2 {
    unsigned int :4;
    unsigned int MMS:3;
    unsigned int :25;
};
struct TIMx_DIER{
    unsigned int UIE:1;
    unsigned int :7;
    unsigned int UDE:1;
    unsigned int :23;
};

typedef struct{
    volatile struct TIMx_CR1 CR1;
    volatile struct TIMx_CR2 CR2;
    volatile uint32_t RESERVED0;
    volatile struct TIMx_DIER DIER;              //DMA/Interrupt enable register
    volatile uint32_t SR;
    volatile uint32_t EGR;               //event generation register
    volatile uint32_t RESERVED1[3];
    volatile uint32_t CNT;               // counter
    volatile uint32_t PSC;               // prescaler
    volatile uint32_t ARR;               // auto-reload register

}BasicTimer_RegDef_t;




//Peripheral definitions 
#define TIM6 ((BasicTimer_RegDef_t *)TIM6_BASEADDR)
#define TIM7 ((BasicTimer_RegDef_t *)TIM7_BASEADDR)

/*
This is a Configuration structure for TIMx x=6,7
*/

typedef struct{
    BasicTimer_RegDef_t* TIMERx;
    unsigned int TIMER_CEN:1;
    unsigned int TIMER_UDIS:1;
    unsigned int TIMER_URS:1;
    unsigned int TIMER_OPM:1;
    unsigned int TIMER_APRE:1;
    unsigned int TIMER_UIFREMA:1;
    unsigned int TIMER_MMS:3;
    unsigned int TIMER_UIE:1;
    unsigned int TIMER_UDE:1;
    uint32_t TIMER_SR;
    uint32_t TIMER_EGR;
    unsigned int :3;
    unsigned int :16; 
    uint16_t TIMER_PSC;
    uint16_t TIMER_ARR;
    
}BasicTimer_Config_t;


void BasicTimer_Init(BasicTimer_Config_t * pBTimCongif);
void BasicTimer_start(BasicTimer_Config_t* tim);
void BasicTimer_DeInit(BasicTimer_Config_t* tim);
void BasicTimerInterrupt_Handling(BasicTimer_RegDef_t* tim);
void BasicTimer_ITConfig(BasicTimer_Config_t* tim, uint8_t EnorDi);
void BasicTimer_IRQPriorityConfig(BasicTimer_Config_t* tim, uint8_t IRQPriority);
/* INC_BASIC_TIMER_H_ */
#endif 
