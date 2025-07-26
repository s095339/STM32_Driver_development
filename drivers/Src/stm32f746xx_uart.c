/*
 * stm32f746xx_uart.c
 *
 *  Created on: Jul 25, 2025
 *      Author: s0953
 */


#include "stm32f746xx_uart.h"

static void set_baudrate(UART_RegDef_t* pUARTx, uint32_t buadrate);
/*
 * Peripheral Clock setup
 */
void UART_PeriClockControl(UART_RegDef_t *pUARTx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pUARTx == USART1)
        {
            USART1_PLCK_EN();
        }else if(pUARTx == USART2)
        {
            USART2_PLCK_EN();
        }else if(pUARTx == USART3)
        {
            USART3_PLCK_EN();
        }else if(pUARTx == UART4)
        {
            UART4_PLCK_EN();
        }else if(pUARTx == UART5)
        {
            UART5_PLCK_EN();
        }else if(pUARTx == USART6)
        {
            USART6_PLCK_EN();
        }else if(pUARTx == UART7)
        {
            UART7_PLCK_EN();
        }else if(pUARTx == UART8)
        {
            UART8_PLCK_EN();
        }
    }
    else
    {
        if(pUARTx == USART1)
        {
            USART1_PLCK_DI();
        }else if(pUARTx == USART2)
        {
            USART2_PLCK_DI();
        }else if(pUARTx == USART3)
        {
            USART3_PLCK_DI();
        }else if(pUARTx == UART4)
        {
            UART4_PLCK_DI();
        }else if(pUARTx == UART5)
        {
            UART5_PLCK_DI();
        }else if(pUARTx == USART6)
        {
            USART6_PLCK_DI();
        }else if(pUARTx == UART7)
        {
            UART7_PLCK_DI();
        }else if(pUARTx == UART8)
        {
            UART8_PLCK_DI();
        }
    }
}

/*
 * Init and De-init
 */
void UART_Init(UART_Handle_t *pUARTHandle)
{
	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	//Implement the code to enable the Clock for given UART peripheral
	UART_PeriClockControl(pUARTHandle->pUARTx, ENABLE);

	//Enable UART Tx and Rx engines according to the UART_Mode configuration item
	if( pUARTHandle->UART_Config.UART_Mode == UART_MODE_ONLY_RX)
	{
		
		tempreg|= (1 << UART_CR1_RE);
	}else if (pUARTHandle->UART_Config.UART_Mode == UART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field 
		tempreg |= ( 1 << UART_CR1_TE );

	}else if (pUARTHandle->UART_Config.UART_Mode == UART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields 
		tempreg |= ( ( 1 << UART_CR1_RE) | ( 1 << UART_CR1_TE) );
	}

    //Implement the code to configure the Word length configuration item 
	tempreg |= pUARTHandle->UART_Config.UART_WordLength << pUARTHandle->UART_Config.UART_WordLength ;


    //Configuration of parity control bit fields
	if ( pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EN_EVEN)
	{
		//Implement the code to enale the parity control 
		tempreg |= ( 1 << UART_CR1_PCE);

		//Implement the code to enable EVEN parity 
		//Not required because by default EVEN parity will be selected once you enable the parity control 

	}else if (pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_EN_ODD )
	{
		//Implement the code to enable the parity control 
	    tempreg |= ( 1 << UART_CR1_PCE);

	    //Implement the code to enable ODD parity 
	    tempreg |= ( 1 << UART_CR1_PS);

	}

   //Program the CR1 register 
	pUARTHandle->pUARTx->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//Implement the code to configure the number of stop bits inserted during UART frame transmission 
	tempreg |= pUARTHandle->UART_Config.UART_NoOfStopBits << UART_CR2_STOP;

	//Program the CR2 register 
	pUARTHandle->pUARTx->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;
	
	//Configuration of UART hardware flow control 
	if ( pUARTHandle->UART_Config.UART_HWFlowControl == UART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to enable CTS flow control 
		tempreg |= ( 1 << UART_CR3_CTSE);


	}else if (pUARTHandle->UART_Config.UART_HWFlowControl == UART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to enable RTS flow control 
		tempreg |= ( 1 << UART_CR3_RTSE);

	}else if (pUARTHandle->UART_Config.UART_HWFlowControl == UART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both CTS and RTS Flow control 
		tempreg |= ( 1 << UART_CR3_CTSE);
        tempreg |= ( 1 << UART_CR3_RTSE);
	}


	pUARTHandle->pUARTx->CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	set_baudrate(pUARTHandle->pUARTx, pUARTHandle->UART_Config.UART_Baud);


    //enable uart
    UART_PeripheralControl(pUARTHandle->pUARTx, ENABLE);

}
void UART_DeInit(UART_RegDef_t *pUARTx)
{

}


/*
 * Data Send and Receive
 */
void UART_SendData(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    
	uint16_t *pdata;
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR
		while(! UART_GetFlagStatus(pUARTHandle->pUARTx,UART_TXE_FLAG));

         //Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			//if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits 
			pdata = (uint16_t*) pTxBuffer;
			pUARTHandle->pUARTx->TDR = (*pdata & (uint16_t)0x01FF);
			
			//check for USART_ParityControl
			if(pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
			{
				//No parity is used in this transfer. so, 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice 
                //因為你可能真的要傳9bit 
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so , 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer 
			pUARTHandle->pUARTx->TDR = (*pTxBuffer  & (uint8_t)0xFF);
			
			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while( ! UART_GetFlagStatus(pUARTHandle->pUARTx,UART_TC_FLAG));
}
void UART_ReceiveData(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until RXNE flag is set in the SR
		while(!UART_GetFlagStatus(pUARTHandle->pUARTx, UART_ISR_RXNE))

		//Check the UART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(pUARTHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
		{
			//We are going to receive 9bit data in a frame

			//check are we using UART_ParityControl control or not
			if(pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
			{
				//No parity is used. so, all 9bits will be of user data

				//read only first 9 bits. so, mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUARTHandle->pUARTx->RDR  & (uint16_t)0x01FF);

				//Now increment the pRxBuffer two times
				pRxBuffer+=2;
			}
			else
			{
				//Parity is used, so, 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (pUARTHandle->pUARTx->RDR  & (uint8_t)0xFF);
				 
				 //Increment the pRxBuffer
				pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//check are we using UART_ParityControl control or not
			if(pUARTHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				 *pRxBuffer = (pUARTHandle->pUARTx->RDR & (uint8_t)0xFF );
			}

			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = ((uint8_t) pUARTHandle->pUARTx->RDR & (uint8_t)0X7F);

			}

			//increment the pRxBuffer
			pRxBuffer++;
		}
	}

}


/*********************************************************************
 * @fn      		  - USART_SendDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs 

 */
uint8_t UART_SendDataIT(UART_Handle_t *pUARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t txstate = pUARTHandle->TxState;

	if(txstate != UART_BUSY_IN_TX)
	{
		pUARTHandle->TxState = UART_BUSY_IN_TX;
		pUARTHandle->pTxBuffer = pTxBuffer;
		pUARTHandle->TxLen = Len;

		//Implement the code to enable interrupt for TXE
		
		pUARTHandle->pUARTx->CR1 |= 1 << UART_CR1_TXEIE;

		//Implement the code to enable interrupt for TC 
		pUARTHandle->pUARTx->CR1 |= 1 << UART_CR1_TCIE;	
		

	}

	return txstate;
}
uint8_t UART_ReceiveDataIT(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUARTHandle->RxState;

	if(rxstate != UART_BUSY_IN_RX)
	{
		pUARTHandle->RxLen = Len;
		pUARTHandle->pRxBuffer = pRxBuffer;
		pUARTHandle->RxState = UART_BUSY_IN_RX;

		//Implement the code to enable interrupt for RXNE
		pUARTHandle->pUARTx->CR1 |= 1 << UART_CR1_RXNEIE;

	}

	return rxstate;
}

/*
 * IRQ Configuration and ISR handling
 */
void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
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
void UART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
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
void UART_IRQHandling(UART_Handle_t *pHandle)
{
    

    if( UART_GetFlagStatus(pHandle->pUARTx, UART_RXNE_FLAG) && pHandle->pUARTx->CR1 & 1<<UART_CR1_RXNEIE )
    {
        if(pHandle->RxState == UART_BUSY_IN_RX && (pHandle->RxLen))
        {
            //Check the UART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
            if(pHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
            {
                //We are going to receive 9bit data in a frame

                //check are we using UART_ParityControl control or not
                if(pHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
                {
                    //No parity is used. so, all 9bits will be of user data

                    //read only first 9 bits. so, mask the DR with 0x01FF
                    *((uint16_t*)(pHandle->pRxBuffer)) = (pHandle->pUARTx->RDR  & (uint16_t)0x01FF);

                    //Now increment the pRxBuffer two times
                    pHandle->pRxBuffer+=2;
                    pHandle->RxLen-=2;
                }
                else
                {
                    //Parity is used, so, 8bits will be of user data and 1 bit is parity
                    *(pHandle->pRxBuffer) = (pHandle->pUARTx->RDR  & (uint8_t)0xFF);
                    
                    //Increment the pRxBuffer
                    pHandle->pRxBuffer++;
                    pHandle->RxLen--;
                }
            }
            else
            {
                //We are going to receive 8bit data in a frame

                //check are we using UART_ParityControl control or not
                if(pHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
                {
                    //No parity is used , so all 8bits will be of user data

                    //read 8 bits from DR
                    *(pHandle->pRxBuffer) = (pHandle->pUARTx->RDR & (uint8_t)0xFF );
                    
                }
                else
                {
                    //Parity is used, so , 7 bits will be of user data and 1 bit is parity

                    //read only 7 bits , hence mask the DR with 0X7F
                    *(pHandle->pRxBuffer) = ((uint8_t) pHandle->pUARTx->RDR & (uint8_t)0X7F);
                    
                }

                //increment the pRxBuffer
                pHandle->pRxBuffer++;
                pHandle->RxLen--;
            }

            if(pHandle->RxLen==0)
            {
                //Implement the code to enable interrupt for TXE
		
                pHandle->pUARTx->CR1 &= ~(1 << UART_CR1_RXNEIE);

                //Implement the code to enable interrupt for TC 
                
                pHandle->RxState = UART_READY;

                UART_ApplicationEventCallback(pHandle, UART_EV_RX_COMPLT);
            }
        }
        /*
        else if(pHandle->RxState == UART_BUSY_IN_RX && (pHandle->RxLen == UART_RXLEN_UNKNOWN))
        {
            //Check the UART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
            if(pHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
            {
                //We are going to receive 9bit data in a frame

                //check are we using UART_ParityControl control or not
                if(pHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
                {
                    //TODO
                }
                else
                {
                    //Parity is used, so, 8bits will be of user data and 1 bit is parity
                    uint8_t temp = (pHandle->pUARTx->RDR  & (uint8_t)0xFF);
                   
                    *(pHandle->pRxBuffer) = temp;
                    


                    
                    // end of reception
                    if( (char)temp == '\n' || (char)temp == '\0'  )
                    {
                        //Implement the code to enable interrupt for TXE
		
                        pHandle->pUARTx->CR1 &= ~(1 << UART_CR1_RXNEIE);

                        //Implement the code to enable interrupt for TC 
                      
                        pHandle->RxState = UART_READY;

                        UART_ApplicationEventCallback(pHandle, UART_EV_RX_COMPLT);
                    }

                    //Increment the pRxBuffer
                    pHandle->pRxBuffer++;
                    
                    
                }
            }
            else
            {
                //We are going to receive 8bit data in a frame

                //check are we using UART_ParityControl control or not
                if(pHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
                {
                    //No parity is used , so all 8bits will be of user data

                    //read 8 bits from DR
                    uint8_t temp = (pHandle->pUARTx->RDR & (uint8_t)0xFF );
                    *(pHandle->pRxBuffer) = temp;
                    // end of reception
                    // end of reception
                    if( (char)temp == '\n' || (char)temp == '\0'  )
                    {
                        //Implement the code to enable interrupt for TXE
		
                        pHandle->pUARTx->CR1 &= ~(1 << UART_CR1_RXNEIE);

                        //Implement the code to enable interrupt for TC 
                      
                        pHandle->RxState = UART_READY;

                        UART_ApplicationEventCallback(pHandle, UART_EV_RX_COMPLT);
                    }
                }
                else
                {
                   //TODO
                    
                }

                //increment the pRxBuffer
                pHandle->pRxBuffer++;
            }
        }
        */
    }


    if( UART_GetFlagStatus(pHandle->pUARTx, UART_TXE_FLAG) && pHandle->pUARTx->CR1 & 1<<UART_CR1_TXEIE  )
    {
        if(pHandle->TxState == UART_BUSY_IN_TX && pHandle->TxLen)
        {

                        //Check the USART_WordLength item for 9BIT or 8BIT in a frame
            if(pHandle->UART_Config.UART_WordLength == UART_WORDLEN_9BITS)
            {
                //if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
                uint16_t *pdata = (uint16_t*)( pHandle->pTxBuffer);
                pHandle->pUARTx->TDR = (*pdata & (uint16_t)0x01FF);

                //check for USART_ParityControl
                if(pHandle->UART_Config.UART_ParityControl == UART_PARITY_DISABLE)
                {
                    //No parity is used in this transfer. so, 9bits of user data will be sent
                    //Implement the code to increment pTxBuffer twice
                    //因為你可能真的要傳9bit
                    pHandle->pTxBuffer+=2;
                    pHandle->TxLen-=2;
                }
                else
                {
                    //Parity bit is used in this transfer . so , 8bits of user data will be sent
                    //The 9th bit will be replaced by parity bit by the hardware
                    pHandle->pTxBuffer++;
                    pHandle->TxLen--;
                }
            }
            else
            {
                //This is 8bit data transfer
                pHandle->pUARTx->TDR = (*(pHandle->pTxBuffer)  & (uint8_t)0xFF);

                //Implement the code to increment the buffer address
                pHandle->pTxBuffer++;
                pHandle->TxLen--;
            }
            //TODO:７bit mode




            if(pHandle->TxLen<=0)
            {
                //Implement the code to enable interrupt for TXE

		        pHandle->pUARTx->CR1 &= ~(1 << UART_CR1_TXEIE);
                UART_ApplicationEventCallback(pHandle, UART_EV_TX_COMPLT);
            }
        }
    }



    if( UART_GetFlagStatus(pHandle->pUARTx, UART_TC_FLAG) && (pHandle->pUARTx->CR1 & 1<<UART_CR1_TCIE) )
    {
        UART_ClearFlag(pHandle->pUARTx, UART_TCCF_CRFLAG);
        
		//Implement the code to enable interrupt for TC 
		pHandle->pUARTx->CR1 &= ~(1 << UART_CR1_TCIE);	
        pHandle->TxState = UART_READY;
    }

}

/*
 * Other Peripheral Control APIs
 */
void UART_PeripheralControl(UART_RegDef_t *pUARTx, uint8_t EnOrDi)
{
    if(EnOrDi == ENABLE)
        pUARTx->CR1 |= 1<<UART_CR1_UE;
    else
        pUARTx->CR1 &= ~(1<<UART_CR1_UE);
}
uint8_t UART_GetFlagStatus(UART_RegDef_t *pUARTx , uint32_t FlagName)
{
    if(pUARTx->ISR & FlagName)// FlagName 其實就是 bit mask
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}
void UART_ClearFlag(UART_RegDef_t *pUARTx, uint16_t clearFlag)
{
    pUARTx->ICR |= clearFlag;
}

/*
 * Application callback
 */
__attribute__((weak)) void UART_ApplicationEventCallback(UART_Handle_t *pUARTHandle,uint8_t AppEv)
{

}



//private funciton

static void set_baudrate(UART_RegDef_t* pUARTx, uint32_t buadrate)
{
    //Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	
    uint32_t temp=0;

    //Get the value of APB bus clock in to the variable PCLKx
    if(pUARTx == USART1 || pUARTx == USART6)
    {
        //USART1 and USART6 are hanging on APB2 bus
        PCLKx = RCC_GetPCLK2Value();
    }else
    {
        PCLKx = RCC_GetPCLK1Value();
    }

    //Check for OVER8 configuration bit
    if(pUARTx->CR1 & (1 << UART_CR1_OVER8))
    {
        //OVER8 = 1 , over sampling by 8
        usartdiv = (2 * PCLKx / buadrate);
    }else
    {
        //over sampling by 16
        usartdiv = (PCLKx / buadrate);
    }

    
    uint32_t tempreg =0;
    if(pUARTx->CR1 & (1 << UART_CR1_OVER8))
    {

        temp = usartdiv;
        temp = (temp>>4)& 0xFFF;
        temp  = temp<<4;
        usartdiv = (usartdiv >> 1) & 0x7;

        tempreg |= temp;
        tempreg |= usartdiv;
        //OVER8 = 1 , over sampling by 8
        pUARTx->BRR = (tempreg & 0xFFFF);
    }else{
        pUARTx->BRR = (usartdiv & 0xFFFF);
    }
    
}
