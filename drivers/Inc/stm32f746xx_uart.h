/*
 * stm32f746xx_uart.h
 *
 *  Created on: Jul 25, 2025
 *      Author: s0953
 */

#ifndef INC_STM32F746XX_UART_H_
#define INC_STM32F746XX_UART_H_
#include "stm32f746xx.h"
typedef struct
{
    uint8_t UART_Mode;
    uint32_t UART_Baud;
    uint8_t UART_NoOfStopBits;
    uint8_t UART_WordLength;
    uint8_t UART_ParityControl;
    uint8_t UART_HWFlowControl;
}UART_Config_t;

typedef struct
{
    UART_RegDef_t * pUARTx;
    UART_Config_t   UART_Config;
    uint8_t         *pTxBuffer;     //store the app. tx buffer address
    uint8_t         *pRxBuffer;   
    uint8_t         TxLen;          
    uint8_t         RxLen;// if RxLen < 0 : the length of data is unknown                  
    uint8_t         TxState;
    uint8_t         RxState;
}UART_Handle_t;

//API

/*
 *@RxLen
 */
#define UART_RXLEN_UNKNOWN -5
/*
 *@UART_Mode
 *Possible options for UART_Mode
 */
#define UART_MODE_ONLY_TX 0
#define UART_MODE_ONLY_RX 1
#define UART_MODE_TXRX  2

/*
 *@UART_Baud
 *Possible options for UART_Baud
 */

#define UART_STD_BAUD_9600					9600
#define UART_STD_BAUD_19200 				19200
#define UART_STD_BAUD_38400 				38400
#define UART_STD_BAUD_57600 				57600
#define UART_STD_BAUD_115200 				115200
#define UART_STD_BAUD_230400 				230400
#define UART_STD_BAUD_460800 				460800
#define UART_STD_BAUD_921600 				921600
#define UART_STD_BAUD_2M 					2000000
#define UART_STD_BAUD_3M 					3000000


/*
 *@UART_ParityControl
 *Possible options for UART_ParityControl
 */
#define UART_PARITY_EN_ODD   2
#define UART_PARITY_EN_EVEN  1
#define UART_PARITY_DISABLE   0

/*
 *@UART_WordLength
 *Possible options for UART_WordLength
 */
#define UART_WORDLEN_8BITS  0
#define UART_WORDLEN_9BITS  1
#define UART_WORDLEN_7BITS  2

/*
 *@UART_NoOfStopBits
 *Possible options for UART_NoOfStopBits
 */
#define UART_STOPBITS_1     0
#define UART_STOPBITS_0_5   1
#define UART_STOPBITS_2     2
#define UART_STOPBITS_1_5   3

/*
 *@UART_HWFlowControl
 *Possible options for UART_HWFlowControl
 */
#define UART_HW_FLOW_CTRL_NONE    	0
#define UART_HW_FLOW_CTRL_CTS    	1
#define UART_HW_FLOW_CTRL_RTS    	2
#define UART_HW_FLOW_CTRL_CTS_RTS	3



// UART flag
#define UART_PE_FLAG        (1U << UART_ISR_PE)
#define UART_FE_FLAG        (1U << UART_ISR_FE)
#define UART_NE_FLAG        (1U << UART_ISR_NE)
#define UART_ORE_FLAG       (1U << UART_ISR_ORE)
#define UART_IDLE_FLAG      (1U << UART_ISR_IDLE)
#define UART_RXNE_FLAG      (1U << UART_ISR_RXNE)
#define UART_TC_FLAG        (1U << UART_ISR_TC)
#define UART_TXE_FLAG       (1U << UART_ISR_TXE)
#define UART_LBDF_FLAG      (1U << UART_ISR_LBDF)
#define UART_CTSIF_FLAG     (1U << UART_ISR_CTSIF)
#define UART_CTS_FLAG       (1U << UART_ISR_CTS)
#define UART_RTOF_FLAG      (1U << UART_ISR_RTOF)
#define UART_EOBF_FLAG      (1U << UART_ISR_EOBF)
#define UART_ABRE_FLAG      (1U << UART_ISR_ABRE)
#define UART_ABRF_FLAG      (1U << UART_ISR_ABRF)
#define UART_BUSY_FLAG      (1U << UART_ISR_BUSY)
#define UART_CMF_FLAG       (1U << UART_ISR_CMF)
#define UART_SBKF_FLAG      (1U << UART_ISR_SBKF)
#define UART_RWU_FLAG       (1U << UART_ISR_RWU)
#define UART_TEACK_FLAG     (1U << UART_ISR_TEACK)

//UART clear flag (好像不用)
#define UART_PECF_CRFLAG    (1U << UART_ICR_PECF)
#define UART_FECF_CRFLAG    (1U << UART_ICR_FECF)
#define UART_NCF_CRFLAG     (1U << UART_ICR_NCF)
#define UART_ORECF_CRFLAG   (1U << UART_ICR_ORECF)
#define UART_IDLECF_CRFLAG  (1U << UART_ICR_IDLECF)
#define UART_TCCF_CRFLAG    (1U << UART_ICR_TCCF)
#define UART_LBDCF_CRFLAG   (1U << UART_ICR_LBDCF)
#define UART_CTSCF_CRFLAG   (1U << UART_ICR_CTSCF)
#define UART_RTOCF_CRFLAG   (1U << UART_ICR_RTOCF)
#define UART_EOBCF_CRFLAG   (1U << UART_ICR_EOBCF)
#define UART_CMCF_CRFLAG    (1U << UART_ICR_CMCF)


//UART state
#define UART_READY 0
#define UART_BUSY_IN_TX 1
#define UART_BUSY_IN_RX 2


// UART EVENT
#define UART_EV_RX_COMPLT 1
#define UART_EV_TX_COMPLT 2
/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void UART_PeriClockControl(UART_RegDef_t *pUARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void UART_Init(UART_Handle_t *pUARTHandle);
void UART_DeInit(UART_RegDef_t *pUARTx);


/*
 * Data Send and Receive
 */
void UART_SendData(UART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
void UART_ReceiveData(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);
uint8_t UART_SendDataIT(UART_Handle_t *pUARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t UART_ReceiveDataIT(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void UART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void UART_IRQHandling(UART_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void UART_PeripheralControl(UART_RegDef_t *pUARTx, uint8_t EnOrDi);
uint8_t UART_GetFlagStatus(UART_RegDef_t *pUARTx , uint32_t FlagName);
void UART_ClearFlag(UART_RegDef_t *pUARTx, uint16_t StatusFlagName);

/*
 * Application callback
 */
void UART_ApplicationEventCallback(UART_Handle_t *pUARTHandle,uint8_t AppEv);

/* INC_STM32F746XX_UART_H_ */
#endif 
