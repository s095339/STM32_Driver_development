/*
 * stm32f746xx_gpio.c
 *
 *  Created on: Jun 20, 2025
 *      Author: s0953
 */


#include "stm32f746xx_gpio.h"



/* 
 Peripheral Clock setup
*/
/***************************************************
 * @fn                      - GPIO_PeriClockControl
 * 
 * @brief                   - This function enables or disables peripheral clock for the given GPIO port
 * 
 * @param[in]               - base addressof the gpio peripheral
 * @param[in]               - ENABLE or DISABLE macros
 * @param[in]               -
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)// Given the GPIOx port base address, Enable or disable the clock
{
    if(EnorDi == ENABLE)
    {
        if(pGPIOx == GPIOA)
        {
            GPIOA_PLCK_EN();
        }else if (pGPIOx == GPIOB)
        {
            GPIOB_PLCK_EN();
        }else if (pGPIOx == GPIOC)
        {
            GPIOC_PLCK_EN();
        }else if (pGPIOx == GPIOD)
        {
            GPIOD_PLCK_EN();
        }else if (pGPIOx == GPIOE)
        {
            GPIOE_PLCK_EN();
        }else if (pGPIOx == GPIOF)
        {
            GPIOF_PLCK_EN();
        }else if (pGPIOx == GPIOG)
        {
            GPIOG_PLCK_EN();
        }else if (pGPIOx == GPIOH)
        {
            GPIOH_PLCK_EN();
        }else if (pGPIOx == GPIOI)
        {
            GPIOI_PLCK_EN();
        }else if (pGPIOx == GPIOJ)
        {
            GPIOJ_PLCK_EN();
        }else if (pGPIOx == GPIOK)
        {
            GPIOK_PLCK_EN();
        }
        
    }
    else
    {
        if(pGPIOx == GPIOA)
        {
            GPIOA_PLCK_DI();
        }else if (pGPIOx == GPIOB)
        {
            GPIOB_PLCK_DI();
        }else if (pGPIOx == GPIOC)
        {
            GPIOC_PLCK_DI();
        }else if (pGPIOx == GPIOD)
        {
            GPIOD_PLCK_DI();
        }else if (pGPIOx == GPIOE)
        {
            GPIOE_PLCK_DI();
        }else if (pGPIOx == GPIOF)
        {
            GPIOF_PLCK_DI();
        }else if (pGPIOx == GPIOG)
        {
            GPIOG_PLCK_DI();
        }else if (pGPIOx == GPIOH)
        {
            GPIOH_PLCK_DI();
        }else if (pGPIOx == GPIOI)
        {
            GPIOI_PLCK_DI();
        }else if (pGPIOx == GPIOJ)
        {
            GPIOJ_PLCK_DI();
        }else if (pGPIOx == GPIOK)
        {
            GPIOK_PLCK_DI();
        }
    }

}


/*
 Init and De-int
*/


/* 
 Peripheral Clock setup
*/
/***************************************************
 * @fn                      - GPIO_Init
 * 
 * @brief                   - 
 * 
 * @param[in]               - 
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    //使用者利用gpio_handle_t的結構，設定想要的GPIO功能跟pin腳，送到Init
    //init這邊幫他做初始化
    uint32_t temp = 0;
    //1. configure the mode of gpio pin ch6.4.1
    if(pGPIOHandle->GPIO_PinCOnfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        //the non-interrup mode
        temp = (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinMode <<  (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber *2));//一個pin的pin mode有兩個bit
        //write to register
        pGPIOHandle->pGPIOx->MODER &= ~(0x3<<(pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber *2)); //clear the target bit
        //為甚麼要先clear?假設原本MODER是10 那我要讓它改成01 那樣的話
        // pGPIOHandle->pGPIOx->MODER |= temp; 這行會變成 10 == 10 | 01 == 11 會是錯的 所以要事先好好的clear
        pGPIOHandle->pGPIOx->MODER |= temp;
        
    }else
    {
        //the interrupt mode
    }

    temp = 0;
    //2. configure the speed
    temp = (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinSpeed <<  (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber *2));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3<<(pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber *2)); //clear the target bit
    pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    temp = 0;


    //3. configure the pupd settings 
    temp = (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinPuPdControl <<  (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber *2));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3<<(pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber *2)); //clear the target bit
    pGPIOHandle->pGPIOx->PUPDR |= temp;
    temp = 0;

    //4. configure the optype ch6.4.2
    temp = (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinOPType <<  (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x1<<pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber); //clear the target bit
    pGPIOHandle->pGPIOx->PUPDR |= temp;
    temp = 0;
    
    //5. configure the alt functionality  ch6.4.9 and 6.4.10
    if(pGPIOHandle->GPIO_PinCOnfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        temp = (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinAltFunMode <<  (4* (pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber &= 0x07 )));
        if(pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber >7)
        {
            pGPIOHandle->pGPIOx->AFR[1] &= ~(0xF<<pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber*4); //clear the target bit
            pGPIOHandle->pGPIOx->AFR[1] |= temp;
        }
        else
        {
            pGPIOHandle->pGPIOx->AFR[0] &= ~(0xF<<pGPIOHandle->GPIO_PinCOnfig.GPIO_PinNumber*4); //clear the target bit
            pGPIOHandle->pGPIOx->AFR[0] |= temp;
        }
    
        temp = 0;
    }
    

}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{

}
/*reset the register
in UM 5.3.5 RCC AHB1 peripheral reset register(RCC_AHB1RSTR).
RCC provids an useful funciton that help us reset the peripheral. By setting the 
corresponding bit, we can have the corresponding peripherval reset.
So we just need the base address of GPIOx
*/

/*
 Data read and wirte
*/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
ISR handling
*/
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQCHandler(uint8_t PinNumber);
