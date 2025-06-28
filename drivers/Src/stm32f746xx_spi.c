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
        
            pSPIHandle->pSPIx->DR = *pTxBuffer;
            len--;
            pTxBuffer++;
        }
    } 
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

void SPI_SSIConfig(SPI_Handle_t *pSPIHandle, uint8_t EnOrDi){
    
    if(EnOrDi == ENABLE)
    {
        pSPIHandle->pSPIx->CR1 |= (1 << SPIx_CR1_SSI);

    }else{
        pSPIHandle->pSPIx->CR1 &= ~(1 << SPIx_CR1_SSI);

    }

}

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
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t EnorDi);

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
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

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
