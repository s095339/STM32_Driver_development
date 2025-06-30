/*
 * stm32f746xx_spi.c
 *
 *  Created on: Jun 26, 2025
 *      Author: s0953
 */
#include "stm32f746xx_spi.h"

 //clock control
 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pSPIx == SPI1)
        {
            SPI1_PLCK_EN();
        }else if(pSPIx == SPI2I2S2)
        {
            SPI2_I2S2_PLCK_EN();
        }else if(pSPIx == SPI3I2S3)
        {
            SPI3_I2S3_PLCK_EN();
        }else if(pSPIx == SPI4)
        {
            SPI4_PLCK_EN();
        }else if(pSPIx == SPI5)
        {
            SPI5_PLCK_EN();
        }else if(pSPIx == SPI6)
        {
            SPI6_PLCK_EN();
        }

    }else
    {
        if(pSPIx == SPI1)
        {
            SPI1_PLCK_DI();
        }else if(pSPIx == SPI2I2S2)
        {
            SPI2_I2S2_PLCK_DI();
        }else if(pSPIx == SPI3I2S3)
        {
            SPI3_I2S3_PLCK_DI();
        }else if(pSPIx == SPI4)
        {
            SPI4_PLCK_DI();
        }else if(pSPIx == SPI5)
        {
            SPI5_PLCK_DI();
        }else if(pSPIx == SPI6)
        {
            SPI6_PLCK_DI();
        }
    }
}

/*
 Init and De-int
*/
 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    //Enable the clock
    SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
    // SPI_CR1 SPI_CR2
    uint32_t tempreg_CR1 = 0;
    uint32_t tempreg_CR2 = 0;
    //1. device mode
    tempreg_CR1 |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPIx_CR1_MSTR;
    //2. bus config
    if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        //bidi mode cleared 
        tempreg_CR1 &= ~(1<<SPIx_CR1_BIDIMODE);
    }else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        //bidi mode set
        tempreg_CR1 |= (1<<SPIx_CR1_BIDIMODE);
    }else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
        //bidi mode cleared
        tempreg_CR1 &= ~(1<<SPIx_CR1_BIDIMODE);
        //RXONLY bit set
        tempreg_CR1 |= (1<<SPIx_CR1_RXONLY);
    }

    //3. SPI_SclkSpeed;

    tempreg_CR1 |= (pSPIHandle->SPIConfig.SPI_SclkSpeed) << SPIx_CR1_BR;

    //4. SPI_CPOL;
    tempreg_CR1 |= (pSPIHandle->SPIConfig.SPI_CPOL) << SPIx_CR1_CPOL;
    //5. SPI_CPHA;
    tempreg_CR1 |= (pSPIHandle->SPIConfig.SPI_CPHA) << SPIx_CR1_CPHA;

    //7. SPI_SSM;
    tempreg_CR1 |= (pSPIHandle->SPIConfig.SPI_SSM) << SPIx_CR1_SSM;
    pSPIHandle->pSPIx->CR1 = tempreg_CR1;
    


    //4. SPI_DS;
    tempreg_CR2 |= (pSPIHandle->SPIConfig.SPI_DS) << SPIx_CR2_DS;
    pSPIHandle->pSPIx->CR2 = tempreg_CR2;
}
 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */

void SPI_DeInit(SPI_Handle_t *pSPIHandle)
{
    if(pSPIHandle->pSPIx == SPI2I2S2){
       SPI2_REG_RESET();
    }
    else if(pSPIHandle->pSPIx == SPI3I2S3)
    {
        SPI3_REG_RESET();
    }else if(pSPIHandle->pSPIx == SPI1)
    {
        SPI1_REG_RESET();
    }else if(pSPIHandle->pSPIx == SPI4)
    {
        SPI4_REG_RESET();
    }else if(pSPIHandle->pSPIx == SPI5)
    {
        SPI5_REG_RESET();
    }else if(pSPIHandle->pSPIx == SPI6)
    {
        SPI6_REG_RESET();
    }

}

uint8_t SPI_GetFlagStatus(SPI_Handle_t * pSPIHandle, uint32_t FlagName)
{
    if(pSPIHandle->pSPIx->SR & FlagName)// FlagName 其實就是 bit mask
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}


/*
Data Send and Receive
*/

 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - This is the blocking call
 * */
void SPI_SendData(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len)
{
    //blocking API
    while(len>0)
    {
        //1. wait until TXE is set
        while(SPI_GetFlagStatus(pSPIHandle, SPI_TXE_FLAG) == FLAG_RESET);
        //2. check the DS bit in CR2
        // TODO: 只先做8bit跟16bit的
        uint32_t temp_DS = pSPIHandle->pSPIx->CR2 >> SPIx_CR2_DS & 0xF ;
        if( temp_DS>8 ){
            //16bits
            pSPIHandle->pSPIx->DR = *((uint16_t*)pTxBuffer);
            len--;
            len--; //因為送出去2個byte
            (uint16_t*)pTxBuffer++; //這樣原本的pTxBuffer就會等同於+2
        }else{
            //8bits
        
            *((uint8_t*)&(pSPIHandle->pSPIx->DR)) = *((uint8_t*)pTxBuffer);
            //必須要把DR給轉型成uint8_t 否則會有dataPACKING的問題 導致系統把這個當作16bits 傳出去

            len--;
            pTxBuffer++;
        }
    } 
}


 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_ReceiveData(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len){
    while(len>0)
    {
        //1. wait until RXNE is set
        while(SPI_GetFlagStatus(pSPIHandle, SPI_RXNE_FLAG) == FLAG_RESET);
        //2. check the DS bit in CR2
        // TODO: 只先做8bit跟16bit的
        uint32_t temp_DS = pSPIHandle->pSPIx->CR2 >> SPIx_CR2_DS & 0xF ;


        if( temp_DS>7 ){
            //16bits
            
            //load the data from DR to Rxbuffer address

            *((uint16_t*)pRxBuffer) = pSPIHandle->pSPIx->DR; 
            len--;
            len--; //因為送出去2個byte
            (uint16_t*)pRxBuffer++; //這樣原本的pTxBuffer就會等同於+2
        }else{
            //8bits
        
            *pRxBuffer = *((uint8_t*)&(pSPIHandle->pSPIx->DR));
            //必須要把DR給轉型成uint8_t 否則會有dataPACKING的問題 導致系統把這個當作16bits 傳出去

            len--;
            pRxBuffer++;
        }
    } 
}

 /***************************************************
 * @fn                      - SPI_SendDataIT
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - Non-blocking API
 * */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len)
{   
    uint8_t state = pSPIHandle->TxState;
    if(state!=SPI_BUSY_IN_TX)
    {
        //1. Save the Tx buffer address and Len information in some global variables
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = len;
        //2. Mark the SPI state as busy in transmission so taht no other code can take over same SPI peripheral until transmission is over
        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        //3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        pSPIHandle->pSPIx->CR2 |= (1<<SPIx_CR2_TXEIE);//(TX Empty interrupt enable)

    }

    return state;
}

 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len)
{
    uint8_t state = pSPIHandle->TxState;
    if(state!=SPI_BUSY_IN_RX)
    {
        //1. Save the Tx buffer address and Len information in some global variables
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->RxLen = len;
        //2. Mark the SPI state as busy in transmission so taht no other code can take over same SPI peripheral until transmission is over
        pSPIHandle->RxState = SPI_BUSY_IN_RX;

        //3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        pSPIHandle->pSPIx->CR2 |= (1<<SPIx_CR2_RXNEIE);//(TX Empty interrupt enable)

    }

    return state;
}
// other API
/***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_PeripheralControl(SPI_Handle_t *pSPIHandle, uint8_t EnOrDi)
{
    if(EnOrDi == ENABLE)
    {
        pSPIHandle->pSPIx->CR1 |= (1 << SPIx_CR1_SPE);

    }else{
        pSPIHandle->pSPIx->CR1 &= ~(1 << SPIx_CR1_SPE);

    }
}
/***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_SSIConfig(SPI_Handle_t *pSPIHandle, uint8_t EnOrDi){
    
    if(EnOrDi == ENABLE)
    {
        pSPIHandle->pSPIx->CR1 |= (1 << SPIx_CR1_SSI);

    }else{
        pSPIHandle->pSPIx->CR1 &= ~(1 << SPIx_CR1_SSI);

    }

}
/***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_SSOEConfig(SPI_Handle_t *pSPIHandle, uint8_t EnOrDi){
    if(EnOrDi == ENABLE)
    {
        pSPIHandle->pSPIx->CR2 |= (1 << SPIx_CR2_SSOE);

    }else{
        pSPIHandle->pSPIx->CR2 &= ~(1 << SPIx_CR2_SSOE);

    }
}


 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_ReceiveData(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);

/*
ISR handling
*/

 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
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

 /***************************************************
 * @fn                      - 
 * 
 * @brief                   
 * 
 * @param[in]              
 * @param[in]               
 *
 * 
 * @return                  - none
 * 
 * @note                    - none
 * */
void SPI_IRQHandling(SPI_Handle_t *pHandle);
