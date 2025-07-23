/*
 * stm32f746xx_i2c.c
 *
 *  Created on: Jul 20, 2025
 *      Author: s0953
 */


#include "stm32f746xx_i2c.h"

//private 
uint16_t AHB_presc[] = {2,4,8,16,64,128,256,512};

// private function
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static uint32_t RCC_GetI2CCLKValue(I2C_Handle_t *I2C_Handle);



//clock control
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
    if(EnorDi == ENABLE)
    {
        if(pI2Cx == I2C1)
        {
            I2C1_PLCK_EN();
        }else if(pI2Cx == I2C2)
        {
            I2C2_PLCK_EN();
        }else if(pI2Cx == I2C3)
        {
            I2C3_PLCK_EN();
        }else if(pI2Cx == I2C4)
        {
            I2C4_PLCK_EN();
        }

    }else
    {
        if(pI2Cx == I2C1)
        {
            I2C1_PLCK_DI();
        }else if(pI2Cx == I2C2)
        {
            I2C2_PLCK_DI();
        }else if(pI2Cx == I2C3)
        {
            I2C3_PLCK_DI();
        }else if(pI2Cx == I2C4)
        {
            I2C4_PLCK_DI();
        }
    }
}

/*
 Init and De-int
*/
void I2C_Init(I2C_Handle_t *pI2CHandle)
{   
    // Enable the clock of I2C
    I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);
    // Disable the i2c  (防呆) 
    I2C_PeripheralControl(pI2CHandle, DISABLE);

    // get clock info
    uint32_t clkfreq = RCC_GetI2CCLKValue(pI2CHandle);
    // set clk source
    if(pI2CHandle->pI2Cx == I2C1)
    {
        RCC->DCKCFGR2 &= ~(pI2CHandle->I2C_Config.I2C_ClkSrc << 16);
        RCC->DCKCFGR2 |= (pI2CHandle->I2C_Config.I2C_ClkSrc << 16);
    }
    else if(pI2CHandle->pI2Cx == I2C2)
    {
        RCC->DCKCFGR2 &= ~(pI2CHandle->I2C_Config.I2C_ClkSrc << 18);
        RCC->DCKCFGR2 |= (pI2CHandle->I2C_Config.I2C_ClkSrc << 18);
    }else if(pI2CHandle->pI2Cx == I2C3)
    {
        RCC->DCKCFGR2 &= ~(pI2CHandle->I2C_Config.I2C_ClkSrc << 20);
        RCC->DCKCFGR2 |= (pI2CHandle->I2C_Config.I2C_ClkSrc << 20);
    }else if(pI2CHandle->pI2Cx == I2C4)
    {
        RCC->DCKCFGR2 &= ~(pI2CHandle->I2C_Config.I2C_ClkSrc << 22);
        RCC->DCKCFGR2 |= (pI2CHandle->I2C_Config.I2C_ClkSrc << 22);
    }else{
        return;
    }


    
    uint32_t tempreg = 0;
    //ack control bit
    //tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << I2C_.. 
    //我這顆MCU好像不需要特別去開啟ACK

    //configure the timing
    //這邊也和課程不一樣
    //但因為設定實在太過於繁瑣，所以這邊用example來做

    // DNF and ANFOFF
    //不會用 先讓他default
    pI2CHandle->pI2Cx->CR1 |= 1 << I2C_CR1_DNF;//digital filter
    //30.4.9 I2C controller mode

    // assume Timing setting for  fI2CCLK of 16 MHz
    switch (pI2CHandle->I2C_Config.I2C_SCLSpeed) {
        case I2C_SCL_SPEED_SM10k:
            tempreg |= (0x3 <<  I2C_TIMINGR_PRESC);  // PRESC
            tempreg |= (0x4 <<  I2C_TIMINGR_SCLDEL);  // SCLDEL
            tempreg |= (0x2 <<  I2C_TIMINGR_SDADEL);  // SDADEL
            tempreg |= (0xC3 << I2C_TIMINGR_SCLH);  // SCLH
            tempreg |= (0xC7 << I2C_TIMINGR_SCLL);  // SCLL
            break;

        case I2C_SCL_SPEED_SM100k:
            tempreg |= (0x3 << I2C_TIMINGR_PRESC);  // PRESC
            tempreg |= (0x4 << I2C_TIMINGR_SCLDEL);  // SCLDEL
            tempreg |= (0x2 << I2C_TIMINGR_SDADEL);  // SDADEL
            tempreg |= (0x0F <<I2C_TIMINGR_SCLH);  // SCLH
            tempreg |= (0x13 <<I2C_TIMINGR_SCLL);  // SCLL
            break;

        case I2C_SCL_SPEED_FM:
            tempreg |= (0x1 << I2C_TIMINGR_PRESC);  // PRESC
            tempreg |= (0x3 << I2C_TIMINGR_SCLDEL);  // SCLDEL
            tempreg |= (0x2 << I2C_TIMINGR_SDADEL);  // SDADEL
            tempreg |= (0x03 <<I2C_TIMINGR_SCLH);  // SCLH
            tempreg |= (0x09 <<I2C_TIMINGR_SCLL);  // SCLL
            break;

        case I2C_SCL_SPEED_FMPlus:
            tempreg |= (0x0 << I2C_TIMINGR_PRESC);  // PRESC
            tempreg |= (0x2 << I2C_TIMINGR_SCLDEL);  // SCLDEL
            tempreg |= (0x0 << I2C_TIMINGR_SDADEL);  // SDADEL
            tempreg |= (0x02 <<I2C_TIMINGR_SCLH);  // SCLH
            tempreg |= (0x04 <<I2C_TIMINGR_SCLL);  // SCLL
            break;

        default:
            // fallback
            break;
    }
    pI2CHandle->pI2Cx->TIMINGR = tempreg;


    //program the device own address (slave address or Target address in the manual)
    tempreg = 0;
    if(pI2CHandle->I2C_Config.I2C_AddrMode)
    {
        //10bits
        tempreg |= 1 << I2C_OAR1_OA1MODE;
        tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress & 0x3FF) << I2C_OAR1_OA1;
    }else
    {
        //7bits
        tempreg &= ~(1 << I2C_OAR1_OA1MODE);
        tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress & 0x7F) << (I2C_OAR1_OA1+1);
    }

    pI2CHandle->pI2Cx->OAR1 = tempreg;

}
void I2C_DeInit(I2C_Handle_t *pI2CHandle){
    
}   


/*
Data Send and Receive
*/
//blocking-based
void I2C_ControllerSendData(
    I2C_Handle_t *pI2CHandle, 
    uint8_t *pTxbuffer, 
    uint32_t Len, 
    uint8_t SlaveAddr, 
    
    uint8_t AUTOEND
)
{
    // W/~R
    //pI2CHandle->pI2Cx->CR2 |= 1<<I2C_CR2_RD_WRN;
    pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_RD_WRN);

    // NBYTE
    pI2CHandle->pI2Cx->CR2 &=  ~(0xFF<<I2C_CR2_NBYTES);
    pI2CHandle->pI2Cx->CR2 |=   (Len<<I2C_CR2_NBYTES);

    //autoend = 1; 
    // TODO: autoend = 0; 
    pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_AUTOEND);

    if(Len <= 255){ 
        pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_RELOAD);
    }
    else{
        pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_RELOAD);
    }

    // Configure target address

    if(pI2CHandle->I2C_Config.I2C_AddrMode)
    {
        //10bits
        pI2CHandle->pI2Cx->CR2 &= ~(0x3FF<<I2C_CR2_SADD);
        pI2CHandle->pI2Cx->CR2 |= ( (SlaveAddr&0x3FF)<<I2C_CR2_SADD);


    }else
    {
        //7bits
        pI2CHandle->pI2Cx->CR2 &= ~(0x7F<<(I2C_CR2_SADD+1));
        pI2CHandle->pI2Cx->CR2 |= ( (SlaveAddr&0x7F)<<(I2C_CR2_SADD+1));
    }


    //Enable I2C
    I2C_PeripheralControl(pI2CHandle, ENABLE);
    // Generate the START condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    // 2. 
    

    while(Len)
    {
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG) );   
        
        pI2CHandle->pI2Cx->TXDR = *(pTxbuffer++);
        Len--;
    
    }
    //while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG) );
    while(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_BUSY_FLAG) );

    I2C_PeripheralControl(pI2CHandle, DISABLE);
}

void I2C_ControllerReceiveData(
    I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t AUTOEND
)
{
    // Controller Initialization
    // W/~R
    pI2CHandle->pI2Cx->CR2 |= 1<<I2C_CR2_RD_WRN;
    //pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_RD_WRN);

    // NBYTE
    pI2CHandle->pI2Cx->CR2 &=  ~(0xFF<<I2C_CR2_NBYTES);
    pI2CHandle->pI2Cx->CR2 |=   (Len<<I2C_CR2_NBYTES);

    //autoend = 1; 
    // TODO: autoend = 0; 
    pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_AUTOEND);

    if(Len <= 255){ 
        pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_RELOAD);
    }
    else{
        pI2CHandle->pI2Cx->CR2 |= (1<<I2C_CR2_RELOAD);
    }

    // Configure target address

    if(pI2CHandle->I2C_Config.I2C_AddrMode)
    {
        //10bits
        pI2CHandle->pI2Cx->CR2 &= ~(0x3FF<<I2C_CR2_SADD);
        pI2CHandle->pI2Cx->CR2 |= ( (SlaveAddr&0x3FF)<<I2C_CR2_SADD);


    }else
    {
        //7bits
        pI2CHandle->pI2Cx->CR2 &= ~(0x7F<<(I2C_CR2_SADD+1));
        pI2CHandle->pI2Cx->CR2 |= ( (SlaveAddr&0x7F)<<(I2C_CR2_SADD+1));
    }


    //Enable I2C
    I2C_PeripheralControl(pI2CHandle, ENABLE);
    // Generate the START condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    // 2. 
    

    while(Len)
    {
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG) );
        
        *(pRxbuffer) = pI2CHandle->pI2Cx->RXDR;
        Len--;
        if(Len)pRxbuffer++;
    }
    //while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG) );
    while(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_BUSY_FLAG) );

    I2C_PeripheralControl(pI2CHandle, DISABLE);

}

//non-blocking(Interrupt-based)
uint8_t I2C_SendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t I2C_ReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t len);


/*
Other peripheral control API
*/
void I2C_PeripheralControl(I2C_Handle_t *pI2CHandle, uint8_t EnOrDi)
{
    if(EnOrDi)
        pI2CHandle->pI2Cx->CR1  |= (1<<I2C_CR1_PE);
    else
        pI2CHandle->pI2Cx->CR1  &= ~(1<<I2C_CR1_PE);
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
    if(pI2Cx->ISR & FlagName)// FlagName 其實就是 bit mask
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/*
* application callback
*/
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);
/*
ISR handling
*/
void I2C_IRQITConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
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






static uint32_t RCC_GetI2CCLKValue(I2C_Handle_t *I2C_Handle)
{

    // check I2C clock source
    if(I2C_Handle->I2C_Config.I2C_AddrMode == I2C_CLKSRC_HSI)
        return 16000000;


    //system clock source
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
        SystemClk = 80000000;//我不知道 我亂掰的
    }else
    {
        //SystemClk = RCC_GetPLLOutputClk();
    }
    if(I2C_Handle->I2C_Config.I2C_AddrMode == I2C_CLKSRC_SYSCLK)
        return SystemClk;

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

    temp = (RCC->CFGR >> 4) & 0xF;

    if(temp<4)
    {
        apb1p = 1;
    }else
    {
        apb1p = AHB_presc[temp-4];
    }
    
    pclk1 = (SystemClk/ahbp)/apb1p;

    return pclk1;
}