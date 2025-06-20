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
    //1. configure the mode of gpio pin ch6.4.1

    //2. configure the speed
    //3. configure the pupd settings 
    //4. configure the optype ch6.4.2
    //5. configure the alt functionality
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
