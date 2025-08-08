/*
 * stm32f746xx_i2c.c
 *
 *  Created on: Jul 20, 2025
 *      Author: s0953
 */


#include "stm32f746xx_i2c.h"

//private 


// private function
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);




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
    //uint32_t clkfreq = RCC_GetI2CCLKValue(pI2CHandle);
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
        tempreg |= 1 << I2C_OAR1_OA1EN;
        tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress & 0x3FF) << I2C_OAR1_OA1;
    }else
    {
        //7bits
        tempreg &= ~(1 << I2C_OAR1_OA1MODE);
        tempreg |= 1 << I2C_OAR1_OA1EN;
        tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress & 0x7F) << (I2C_OAR1_OA1+1);
    }

    pI2CHandle->pI2Cx->OAR1 = tempreg;

    I2C_PeripheralControl(pI2CHandle, ENABLE);
    //Interrupt==========================================//
    tempreg = 0;
    // Interrupt enable for possible slave transcation
    tempreg |= 1 << I2C_CR1_ADDRIE;
    // open TXIS interrupt
    //tempreg |= 1 << I2C_CR1_TXIE; 
    // open RXNE interrupt
    //tempreg |= 1 << I2C_CR1_RXIE; 
    // open TC and TCR interrupt
    //tempreg |= 1 << I2C_CR1_TCIE;
    // open STOPIE 
    //tempreg |= 1 << I2C_CR1_STOPIE;
    pI2CHandle->pI2Cx->CR1 |= tempreg;
    
    
    

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
    //disable interrupt========================

    uint32_t tempreg = 0;
    // Interrupt enable for possible slave transcation
    tempreg |= 1 << I2C_CR1_ADDRIE;
 
    pI2CHandle->pI2Cx->CR1 &= ~(tempreg);

    //=========================================
    // R/~W
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

   //enable interrupt========================
    
    tempreg = 0;
    // Interrupt enable for possible slave transcation
    tempreg |= 1 << I2C_CR1_ADDRIE;
   
    pI2CHandle->pI2Cx->CR1 |= (tempreg);

    //=========================================
}

void I2C_ControllerReceiveData(
    I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t AUTOEND
)
{
    //disable interrupt========================
    
    uint32_t tempreg = 0;
    
    tempreg |= 1 << I2C_CR1_ADDRIE;
    
    pI2CHandle->pI2Cx->CR1 &= ~(tempreg);

    //=========================================
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


    //enable interrupt========================
    
    tempreg = 0;
    // Interrupt enable for possible slave transcation
    tempreg |= 1 << I2C_CR1_ADDRIE;

    pI2CHandle->pI2Cx->CR1 |= (tempreg);

    //=========================================

}

//non-blocking(Interrupt-based)
uint8_t I2C_ControllerSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{
    uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

        //Implement the code to enable Interrupt

        uint32_t tempreg = 0;
        // open TXIS interrupt
        tempreg |= 1 << I2C_CR1_TXIE; 
        // open ADDR interrupt (for target mode, when address sent is matched)

        // open STOPF interrupt  (STOP detection flag: This flag is set by hardware when a STOP condition 
        tempreg |= 1 << I2C_CR1_STOPIE;
        // open TC and TCR interrupt
        tempreg |= 1 << I2C_CR1_TCIE;
        //TODO: open bus error, overrun/underrun and timeout error
        //tempreg |= 1 << I2C_CR1_ERRIE;

        pI2CHandle->pI2Cx->CR1 |= tempreg;



        // R/~W
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


        
        
		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		
	}

	return busystate;
}
uint8_t I2C_ControllerReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
    uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception 
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		

		//Implement the code to enable Interrupt

        uint32_t tempreg = 0;
        // open TXIS interrupt
        tempreg |= 1 << I2C_CR1_RXIE; 
        // open TC and TCR interrupt
        tempreg |= 1 << I2C_CR1_TCIE;
        // open STOPIE 
        tempreg |= 1 << I2C_CR1_STOPIE;
        //TODO: open bus error, overrun/underrun and timeout error
        //tempreg |= 1 << I2C_CR1_ERRIE;

		pI2CHandle->pI2Cx->CR1 |= tempreg;

        //initialize
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


        
        //Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	}

	return busystate;
}


void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data)
{
    pI2Cx->TXDR = data;
}
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx)
{
    return pI2Cx->RXDR;
}

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

__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{

}
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
    uint8_t iprx = IRQNumber/4;
    uint8_t iprx_section = IRQNumber %4;
    uint8_t shift_amount = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED);
    /*根據UM 10.1 NVIC features 雖然cortex-M7的processor提供了8bits的中段優先級，
    但是STM32的這個microprocessor只implement了4bits，所以要把IRQPriority右移4bit寫進去*/

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}







void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    uint32_t EV_EN;
    uint32_t EV_FLAG;
    uint32_t MASK;
    EV_EN = pI2CHandle->pI2Cx->CR1;
    
    


    //STOPIE
    MASK = (1 << I2C_CR1_STOPIE);
    if((EV_EN & MASK) == MASK && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_STOPF_FLAG))
    {
        //Clear the STOPF by setting the STOPCF bit
        pI2CHandle->pI2Cx->ICR |= (1 << I2C_ICR_STOPCF);
        //Disable the stop interrupt
        pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_STOPIE);
        //Notify the application that STOP is detected

        //check autoend
        
       


        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            
            if( !( (pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_AUTOEND)) == (1 << I2C_CR2_AUTOEND) ))
            {
                I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
            }
            uint32_t tempreg = 0;
            // disable TXIS interrupt
            tempreg |= 1 << I2C_CR1_TXIE; 
            // disable ADDR interrupt (for target mode, when address sent is matched)

            // disable STOPF interrupt  (STOP detection flag: This flag is set by hardware when a STOP condition 
            tempreg |= 1 << I2C_CR1_STOPIE;
            // disable TC and TCR interrupt
            tempreg |= 1 << I2C_CR1_TCIE;
            //TODO: disable bus error, overrun/underrun and timeout error
            //tempreg |= 1 << I2C_CR1_ERRIE;

            pI2CHandle->pI2Cx->CR1 &= ~(tempreg);
            //I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);

            I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
        }else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            if( !( (pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_AUTOEND)) == (1 << I2C_CR2_AUTOEND) ))
            {
                I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
            }
            uint32_t tempreg = 0;
            // disable RXNE interrupt
            tempreg |= 1 << I2C_CR1_RXIE; 
            // disable TC and TCR interrupt
            tempreg |= 1 << I2C_CR1_TCIE;
            // disable STOPIE 
            tempreg |= 1 << I2C_CR1_STOPIE;
            //TODO: disable bus error, overrun/underrun and timeout error
            //tempreg |= 1 << I2C_CR1_ERRIE;

            pI2CHandle->pI2Cx->CR1 &= ~(tempreg);
            //I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
            
            I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
        }else
        {
            /*
             * Target mode
             * STOPF is generate by Controller
             */
            uint32_t tempreg = 0;
            // disable RXNE interrupt
            tempreg |= 1 << I2C_CR1_RXIE; 
            // disable TXIS interrupt
            tempreg |= 1 << I2C_CR1_TXIE;
            // disable STOPIE 
            tempreg |= 1 << I2C_CR1_STOPIE;

            pI2CHandle->pI2Cx->CR1 &= ~(tempreg);
            I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TARGET_STOP);
        }
        
        pI2CHandle->TxRxState = I2C_READY;

        
    }

    // ADDR (target mode) ========================
    MASK = (1 << I2C_CR1_ADDRIE);
    if((EV_EN & MASK) == MASK && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG))
    {
        // STM32 go into Target Mode
        
        //clear 
        
        //check ADDR (7bits)
        uint32_t ADDR_MASK = 0x7F;
        
        if( ((pI2CHandle->pI2Cx->ISR >>I2C_ISR_ADDCODE)&ADDR_MASK) == ((pI2CHandle->pI2Cx->OAR1 >> (I2C_OAR1_OA1+1))&ADDR_MASK))
        {
            uint32_t DIR_MASK = 1 << I2C_ISR_DIR;
            if((pI2CHandle->pI2Cx->ISR & DIR_MASK) == DIR_MASK)
            {
                //  Read transfer, target enters transmitter mode.
                I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TARGET_TRAN);
                // Data flush, flush TXDR
                pI2CHandle->pI2Cx->ISR |= 1 << I2C_ISR_TXE;
                // Enable TXIS interrupt
                pI2CHandle->pI2Cx->CR1 |= 1 << I2C_CR1_TXIE;
                
            }
            else
            {
                // Write transfer, target enters receiver mode.
                I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TARGET_RCV);
                // Enable RXNE interrupt
                pI2CHandle->pI2Cx->CR1 |= 1 << I2C_CR1_RXIE;
                
            }
            // Enable STOPF interrupt
            pI2CHandle->pI2Cx->CR1 |= 1 << I2C_CR1_STOPIE;
            
        }
        
        // clear addr interrupt
        pI2CHandle->pI2Cx->ICR |= 1 << I2C_ICR_ADDRCF;
    }
    
    // TC ========================================
    MASK = (1 << I2C_CR1_TCIE);
    if( ((EV_EN & MASK) == MASK) && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TC_FLAG))
    {
        //TODO
    }
    // TCR
    MASK = (1 << I2C_CR1_TCIE);
    if( (EV_EN & MASK) == MASK && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TCR_FLAG))
    {
        //TODO:
    }


    //TXIS==========================================//
    MASK = (1 << I2C_CR1_TXIE);
    if( (EV_EN & MASK) == MASK && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXIS_FLAG))
    {
        //if device is master
        
        // Data transmition
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            // Controller mode
            if(pI2CHandle->TxLen > 0)
            {
                //1. load the data into DR (cleared the TXIS and TXE)
                pI2CHandle->pI2Cx->TXDR = *(pI2CHandle->pTxBuffer);
                //2. decrement the TxLen
                pI2CHandle->TxLen--;
                //3. Increment the buffer address
                if(pI2CHandle->TxLen)
                    pI2CHandle->pTxBuffer++;
            }
        }else{
            // Target mode
            /* 
            明明沒有去設定 pI2CHandle->TxRxState = I2C_BUSY_IN_TX卻發生中斷？
            那只有一種可能：現在是target mode的TXIS 中斷
            */

            I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
        }   

    }
    //RXNE==========================================//
    MASK = (1 << I2C_CR1_RXIE);
    if( (EV_EN & MASK) == MASK && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG))
    {
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            //1. read the data in DR
            *(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->RXDR;
            //2. 
            pI2CHandle->RxLen--;
            //3.
            if(pI2CHandle->RxLen)
                pI2CHandle->pRxBuffer++;
            
        }else{
            // Target mode
            /* 
            明明沒有去設定 pI2CHandle->TxRxState = I2C_BUSY_IN_RX卻發生中斷？
            那只有一種可能：現在是target mode的RXNE 中斷
            */
           I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
        }
    }
    
}


//private function
static inline void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->CR2 |= 1 << I2C_CR2_START;
}
static inline void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->CR2 |= 1 << I2C_CR2_STOP;
}
