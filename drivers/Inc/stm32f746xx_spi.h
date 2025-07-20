/*
 * stm32f746xx_spi.h
 *
 *  Created on: Jun 26, 2025
 *      Author: s0953
 */

#ifndef INC_STM32F746XX_SPI_H_
#define INC_STM32F746XX_SPI_H_

#include "stm32f746xx.h"

// configuratiop structure
typedef struct 
{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DS;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
}SPI_Config_t;

//SPI and I2S register
typedef struct {
    SPI_RegDef_t    *pSPIx;
    SPI_Config_t    SPIConfig;
    uint8_t         *pTxBuffer;     //store the app. tx buffer address
    uint8_t         *pRxBuffer;     //stort the app. tx buffer address
    uint8_t         TxLen;          
    uint8_t         RxLen;                  
    uint8_t         TxState;
    uint8_t         RxState;
}SPI_Handle_t;


/*
 * SPI DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE  0

/*
 * SPI BusConfig
 */
#define SPI_BUS_CONFIG_FD                   1
#define SPI_BUS_CONFIG_HD                   2
//#define SPI_BUS_CONFIG_SIMPLEX_TXONLY       3 我們不需要這個 FD其實就包含這個
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY       4

/*
 *SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2                 0
#define SPI_SCLK_SPEED_DIV4                 1
#define SPI_SCLK_SPEED_DIV8                 2
#define SPI_SCLK_SPEED_DIV16                3
#define SPI_SCLK_SPEED_DIV32                4
#define SPI_SCLK_SPEED_DIV64                5
#define SPI_SCLK_SPEED_DIV128               6
#define SPI_SCLK_SPEED_DIV256               7

/*
 *SPI_DFF
 */

#define SPI_DS_4BITS                       3
#define SPI_DS_5BITS                       4
#define SPI_DS_6BITS                       5
#define SPI_DS_7BITS                       6
#define SPI_DS_8BITS                       7
#define SPI_DS_9BITS                       8
#define SPI_DS_10BITS                      9
#define SPI_DS_11BITS                      10
#define SPI_DS_12BITS                      11
#define SPI_DS_13BITS                      12
#define SPI_DS_14BITS                      13
#define SPI_DS_15BITS                      14
#define SPI_DS_16BITS                      15

/*
* CPOL
*/

#define SPI_CPOL_HIGH                       1
#define SPI_CPOL_LOW                        0

/*
*  CPHA
*/
#define SPI_CPHA_HIGH                       1
#define SPI_CPHA_LOW                        0

/*
*  SSM
*/

#define SPI_SSM_DI                          0
#define SPI_SSM_EN                          1

/*
*   SPI related staus flags definitions
*/

#define SPI_RXNE_FLAG                       (1 << SPIx_SR_RXNE)
#define SPI_TXE_FLAG                        (1 << SPIx_SR_TXE)
#define SPI_CHSIDE_FLAG                     (1 << SPIx_SR_CHSIDE)
#define SPI_UDR_FLAG                        (1 << SPIx_SR_UDR)
#define SPI_CRCERR_FLAG                     (1 << SPIx_SR_CRCERR)
#define SPI_MODF_FLAG                       (1 << SPIx_SR_MODF)
#define SPI_OVR_FLAG                        (1 << SPIx_SR_OVR)
#define SPI_BSY_FLAG                        (1 << SPIx_SR_BSY)
#define SPI_FRE_FLAG                        (1 << SPIx_SR_FRE)

//SPI application state
#define SPI_READY                           0
#define SPI_BUSY_IN_RX                      1
#define SPI_BUSY_IN_TX                      2

//possible SPI application events
#define SPI_EVENT_TX_CMPLT                  1
#define SPI_EVENT_RX_CMPLT                  2
#define SPI_EVENT_OVR_ERR                   3
#define SPI_EVENT_CRC_ERR                   4
/***************************************************
 *                      APIs 
 ***************************************************/

 //clock control
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 Init and De-int
*/
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_Handle_t *pSPIHandle);


/*
Data Send and Receive
*/
//blocking-based
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);
//non-blocking(Interrupt-based)
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);


/*
Other peripheral control API
*/
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
/*
* application callback
*/
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);
/*
ISR handling
*/
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);




#endif /* INC_STM32F746XX_SPI_H_ */
