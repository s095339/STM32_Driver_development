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
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        //the non-interrup mode
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<  (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber *2));//一個pin的pin mode有兩個bit
        //write to register
        pGPIOHandle->pGPIOx->MODER &= ~(0x3<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber *2)); //clear the target bit
        //為甚麼要先clear?假設原本MODER是10 那我要讓它改成01 那樣的話
        // pGPIOHandle->pGPIOx->MODER |= temp; 這行會變成 10 == 10 | 01 == 11 會是錯的 所以要事先好好的clear
        pGPIOHandle->pGPIOx->MODER |= temp;
        
    }else
    {
        //the interrupt mode
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            //1. configure the FTSR
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            //clear the corresponding RTSR bit
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            //1. configure the RTSR
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            //clear the corresponding RTSR bit
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            //1. configure both FTSR and RTSR
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            //clear the corresponding RTSR bit
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        //2. configure the GPIO port selection in SYSCFG_EXTICR
        // determind the port of which pin number is used for EXTI
        //ch 7.2.3~7.2.6 and ch 11.8 
        uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4;
        uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4;
        uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
        SYSCFG->EXTICR[temp1] = portcode << (temp2*4);
        //3. enable the exti interrupt delivery using IMR
        EXTI->IMR |= (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    }

    temp = 0;
    //2. configure the speed
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed <<  (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber *2));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber *2)); //clear the target bit
    pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    temp = 0;


    //3. configure the pupd settings 
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl <<  (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber *2));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3<<(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber *2)); //clear the target bit
    pGPIOHandle->pGPIOx->PUPDR |= temp;
    temp = 0;

    //4. configure the optype ch6.4.2
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType <<  (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clear the target bit
    pGPIOHandle->pGPIOx->PUPDR |= temp;
    temp = 0;
    
    //5. configure the alt functionality  ch6.4.9 and 6.4.10
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode <<  (4* (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber &= 0x07 )));
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber >7)
        {
            pGPIOHandle->pGPIOx->AFR[1] &= ~(0xF<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*4); //clear the target bit
            pGPIOHandle->pGPIOx->AFR[1] |= temp;
        }
        else
        {
            pGPIOHandle->pGPIOx->AFR[0] &= ~(0xF<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber*4); //clear the target bit
            pGPIOHandle->pGPIOx->AFR[0] |= temp;
        }
    
        temp = 0;
    }
    

}


/***************************************************
 * @fn                      - GPIO_DeInit
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
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if(pGPIOx == GPIOA)
        {
            GPIOA_REG_RESET();
        }else if (pGPIOx == GPIOB)
        {
            GPIOB_REG_RESET();
        }else if (pGPIOx == GPIOC)
        {
            GPIOC_REG_RESET();
        }else if (pGPIOx == GPIOD)
        {
            GPIOD_REG_RESET();
        }else if (pGPIOx == GPIOE)
        {
            GPIOE_REG_RESET();
        }else if (pGPIOx == GPIOF)
        {
            GPIOF_REG_RESET();
        }else if (pGPIOx == GPIOG)
        {
            GPIOG_REG_RESET();
        }else if (pGPIOx == GPIOH)
        {
            GPIOH_REG_RESET();
        }else if (pGPIOx == GPIOI)
        {
            GPIOI_REG_RESET();
        }else if (pGPIOx == GPIOJ)
        {
            GPIOJ_REG_RESET();
        }else if (pGPIOx == GPIOK)
        {
            GPIOK_REG_RESET();
        }
}
/*reset the register
in UM 5.3.5 RCC AHB1 peripheral reset register(RCC_AHB1RSTR).
RCC provids an useful funciton that help us reset the peripheral. By setting the 
corresponding bit once and resetting that bit(That is, send a pulse to reset pin), we can have the corresponding peripherval reset.  
So we just need the base address of GPIOx
*/

/*
 Data read and wirte
*/


/***************************************************
 * @fn                      - GPIO_ReadFromInputPin
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
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    uint8_t value;
    value = (uint8_t)((pGPIOx -> IDR >> PinNumber) & 0x00000001);
    return value;
}



/***************************************************
 * @fn                      - GPIO_ReadFromInputPort
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
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value;
    value = (uint16_t)(pGPIOx -> IDR);
    return value;
}

/***************************************************
 * @fn                      - GPIO_WriteToOutputPin
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
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
    if(Value == GPIO_PIN_SET)
    {
        pGPIOx->ODR |= 1 << PinNumber;
    }else
    {
        pGPIOx->ODR &= ~(0 << PinNumber);
    }   
}


/***************************************************
 * @fn                      - GPIO_WriteToOutputPort
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
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
    pGPIOx->ODR = Value;
}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
    pGPIOx->ODR ^= 1 << PinNumber;
}

/*
ISR handling
*/


/***************************************************
 * @fn                      - GPIO_IRQITConfig
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
void GPIO_IRQITConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
    /*refer to the ARM® Cortex®-M7 Devices Generic user guide
    4.2 NVIC 
    這邊已經不是micro controlller side 已經接觸到processor side了 所以要去看processer的文件
    以便設定NVIC*/    
    
    
    /****
     * 我們是要去設定processor的 Interrupt Set-enable Registers(ISER) 來打開interrupt
     *  Interrupt Clear-enable Registers(ICER) 來關掉interrupt
     */
    if(EnorDi == ENABLE)
    {
        if(EnorDi == ENABLE)
        {
            if(IRQNumber <= 31)
            {
                //program ISER0 register
                *NVIC_ISER0 |= (1<<IRQNumber);

            }else if(IRQNumber>31 && IRQNumber<64)
            {
                *NVIC_ISER1 |= (1<<IRQNumber%32);
            }else if(IRQNumber>64 && IRQNumber<96)//stm32f746g的interrupt number沒有那麼多
            {
                *NVIC_ISER2 |= (1<<IRQNumber%64);
            }
        }else{
            if(IRQNumber <= 31)
            {
                //program ISER0 register
                *NVIC_ICER0 &= ~(1<<IRQNumber);

            }else if(IRQNumber>31 && IRQNumber<64)
            {
                *NVIC_ICER1 &= ~(1<<IRQNumber%32);
            }else if(IRQNumber>64 && IRQNumber<96)//stm32f746g的interrupt number沒有那麼多
            {
                *NVIC_ICER2 &= ~(1<<IRQNumber%64);
            }
        }
    }
}
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
    //1. first lets find out the ipr register
    // M7 user guide 4.2,  Interrupt Priority Registers
    uint8_t iprx = IRQNumber;
    uint8_t iprx_section = IRQNumber %4;
    uint8_t shift_amount = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);
    /*根據UM 10.1 NVIC features 雖然cortex-M7的processor提供了8bits的中段優先級，
    但是STM32的這個microprocessor只implement了4bits，所以要把IRQPriority右移4bit寫進去*/

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}
void GPIO_IRQHandling(uint8_t PinNumber)
{
    //clear the exti pr register (pending register) to the pin number
    // UM 11.9.6
    if( EXTI->PR & (1<<PinNumber) )
    {
        //clear
        EXTI->PR |= (1<<PinNumber);// 寫1進去來clear UM就是這樣說的
    }
}
