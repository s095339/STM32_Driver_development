/*
 * stm32f746xx_i2c.h
 *
 *  Created on: Jul 20, 2025
 *      Author: s0953
 */

#ifndef INC_STM32F746XX_I2C_H_
#define INC_STM32F746XX_I2C_H_

#include "stm32f746xx.h"
//I2Cs can be clocked by: SYSCLK, HSI, PCLK1
/*(和fastbit課程不一樣)
in chap  5.3.26 RCC dedicated clocks configuration register (DCKCFGR2)
This register allows to select the source clock for the 48MHz, SDMMC, HDMI-CEC, 
LPTIM1, UARTs, USARTs and I2Cs clocks

In this project, I assume that HSI is used and the freq is 16000000

*/
//in this file we assume that the I2C is clocked by PLCK1 (APB1 clock)


//Configuration structure
typedef struct 
{
    uint32_t    I2C_AddrMode;
    uint32_t    I2C_ClkSrc;
    uint32_t    I2C_SCLSpeed;
    uint8_t     I2C_DeviceAddress; //mentioned by the user
    //uint8_t     I2C_ACKControl; //我的板子沒有這個東西
    //uint16_t    I2C_FMDutyCycle;
}I2C_Config_t;

//handle structure
typedef struct
{
    I2C_RegDef_t    *pI2Cx;
    I2C_Config_t    I2C_Config;

    //Non-blocking API
    uint8_t         *pTxBuffer;          // To store the app. Tx buffer address
    uint8_t         *pRxBuffer;          // To store the app. Rx buffer address
    uint32_t        TxLen;              // To store Tx len
    uint32_t        RxLen;              // To store Rx len
    uint8_t         TxRxState;           // To store communication state @TxRxState
    uint8_t         DevAddr;             // To store slave/device address
    uint32_t        RxSize;             // To store Rx size
    uint8_t         Sr;                  // To store repeated start value
    // Non-blocking API for slave
    

}I2C_Handle_t;

/*
 *  @I2C_AddMode;  
 * 
 */
#define I2C_ADDRMODE_7BIT   0
#define I2C_ADDRMODE_10BIT  1

/*
 *  @I2C_ClkSrc;  
 *  
 */
#define I2C_CLKSRC_PCLK1  0
#define I2C_CLKSRC_SYSCLK  1
#define I2C_CLKSRC_HSI  2 //16MB

/*
 *  @I2C_SCLSpeed;  
 *  based on  30.4.10 Table 186
 */

#define I2C_SCL_SPEED_SM10k     10000
#define I2C_SCL_SPEED_SM100k     100000
#define I2C_SCL_SPEED_FM        400000
#define I2C_SCL_SPEED_FMPlus    500000

/*
 *  @I2C_ACKControl;  
 *  based on  30.4.10 Table 186
 */
#define I2C_ACK_ENABLE          1
#define I2C_ACK_DISABLE         0


/*
*   SPI related staus flags definitions
*/
#define I2C_TXE_FLAG       (1 << I2C_ISR_TXE)
#define I2C_TXIS_FLAG      (1 << I2C_ISR_TXIS)
#define I2C_RXNE_FLAG      (1 << I2C_ISR_RXNE)
#define I2C_ADDR_FLAG      (1 << I2C_ISR_ADDR)
#define I2C_NACKF_FLAG     (1 << I2C_ISR_NACKF)
#define I2C_STOPF_FLAG     (1 << I2C_ISR_STOPF)
#define I2C_TC_FLAG        (1 << I2C_ISR_TC)
#define I2C_TCR_FLAG       (1 << I2C_ISR_TCR)
#define I2C_BERR_FLAG      (1 << I2C_ISR_BERR)
#define I2C_ARLO_FLAG      (1 << I2C_ISR_ARLO)
#define I2C_OVR_FLAG       (1 << I2C_ISR_OVR)
#define I2C_PECERR_FLAG    (1 << I2C_ISR_PECERR)
#define I2C_TIMEOUT_FLAG   (1 << I2C_ISR_TIMEOUT)
#define I2C_ALERT_FLAG     (1 << I2C_ISR_ALERT)
#define I2C_BUSY_FLAG      (1 << I2C_ISR_BUSY)
#define I2C_DIR_FLAG       (1 << I2C_ISR_DIR)
//#define I2C_ADDCODE_MASK   (0x7F << I2C_ISR_ADDCODE)

/*
 * @TxRxState
 */
#define I2C_READY        0
#define I2C_BUSY_IN_RX   1
#define I2C_BUSY_IN_TX   2

/*
 * I2C application events macros
 */
#define I2C_EV_TX_CMPLT  	 	0
#define I2C_EV_RX_CMPLT  	 	1
#define I2C_EV_TARGET_STOP       		2
//#define I2C_ERROR_BERR 	 	3
//#define I2C_ERROR_ARLO  		4
//#define I2C_ERROR_AF    		5
//#define I2C_ERROR_OVR   		6
//#define I2C_ERROR_TIMEOUT 	7

#define I2C_EV_DATA_REQ         8
#define I2C_EV_DATA_RCV         9
#define I2C_EV_TARGET_TRAN      10      
#define I2C_EV_TARGET_RCV       11
 /***************************************************
 *                      APIs 
 ***************************************************/

 //clock control
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 Init and De-int
*/
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_Handle_t *pI2CHandle);


/*
Data Send and Receive
*/
//blocking-based
void I2C_ControllerSendData(
    I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t AUTOEND
);

void I2C_ControllerReceiveData(
    I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t AUTOEND
);
//non-blocking(Interrupt-based)
uint8_t I2C_ControllerSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_ControllerReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

/*
Other peripheral control API
*/
void I2C_PeripheralControl(I2C_Handle_t *pI2CHandle, uint8_t EnOrDi);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

/*
* application callback
*/
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);
/*
ISR handling
*/
void I2C_IRQITConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);




/* INC_STM32F746XX_I2C_H_ */
#endif 

 