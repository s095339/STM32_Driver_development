/*
 * stm32f746xx_rcc.c
 *
 *  Created on: Jul 25, 2025
 *      Author: s0953
 */


#include "stm32f746xx_rcc.h"

uint16_t AHB_presc[] = {2,4,8,16,64,128,256,512};
uint16_t APB_presc[] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value()
{

    
    uint32_t    pclk1;
    uint8_t     clksrc, temp;
    uint32_t    SystemClk;
    uint16_t    ahbp,apb1p; //ahb prescalar
    // To check SWS (CFGR[3:2]), System clock switch status這邊決定system clock的來源是甚麼
    clksrc = (RCC->CFGR >> 2) & 0x3;
    if(clksrc == 0) //HSI
    {
        SystemClk = 16000000;
    }else if(clksrc == 1) //HSE
    {
        //TODO:
    }else
    {
        //TODO
    }
    

    // get the CFGR[7:4] ,HPRE (AHB prescaler)
    
    temp = (RCC->CFGR >> 4) & 0xF;

    if(temp<8)
    {
        ahbp = 1;
    }else
    {
        ahbp = AHB_presc[temp-8];
    }

    // get the CFGR[12:10] , PPRE1 (APB Low-speed prescaler (APB1))

    temp = (RCC->CFGR >> 10) & 0x7;

    if(temp<4)
    {
        apb1p = 1;
    }else
    {
        apb1p = APB_presc[temp-4];
    }
    
    pclk1 = (SystemClk/ahbp)/apb1p;

    return pclk1;
}


uint32_t RCC_GetPCLK2Value()
{

    
    uint32_t    pclk2;
    uint8_t     clksrc, temp;
    uint32_t    SystemClk;
    uint16_t    ahbp,apb2p; //ahb prescalar
    // To check SWS (CFGR[3:2]), System clock switch status這邊決定system clock的來源是甚麼
    clksrc = (RCC->CFGR >> 2) & 0x3;
    if(clksrc == 0) //HSI
    {
        SystemClk = 16000000;
    }else if(clksrc == 1) //HSE
    {
        //TODO:
    }else
    {
        //TODO
    }
    

    // get the CFGR[7:4] ,HPRE (AHB prescaler)
    
    temp = (RCC->CFGR >> 4) & 0xF;

    if(temp<8)
    {
        ahbp = 1;
    }else
    {
        ahbp = AHB_presc[temp-8];
    }

    // get the CFGR[15:13] , PPRE1 (APB Low-speed prescaler (APB2))

    temp = (RCC->CFGR >> 13 & 0x7);

    if(temp<4)
    {
        apb2p = 1;
    }else
    {
        apb2p = APB_presc[temp-4];
    }
    
    pclk2 = (SystemClk/ahbp)/apb2p;

    return pclk2;
}