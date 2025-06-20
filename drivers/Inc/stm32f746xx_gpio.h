/*
 * stm32f746xx_gpio.h
 *
 *  Created on: Jun 20, 2025
 *      Author: s0953
 */

#ifndef INC_STM32F746XX_GPIO_H_
#define INC_STM32F746XX_GPIO_H_

#include "stm32f746xx.h"

/*
This is a Configuration structure for a GPIO pin
*/



typedef struct{
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;           /*!<possible values from @GPIO_PIN_MODES>*/
    uint8_t GPIO_PinSpeed;          /*!<possible values from @GPIO_PIN_SPEED>*/
    uint8_t GPIO_PinPuPdControl;    /*!<possible values from @GPIO_PIN_PPC>*/
    uint8_t GPIO_PinOPType;         /*!<possible values from @GPIO_PIN_OPTYPE>*/
    uint8_t GPIO_PinAltFunMode;     /*!<possible values from @GPIO_PIN_AltFunMode>*/
}GPIO_PinConfig_t;
/*
This is a Handle structure for a GPIO pin
*/

typedef struct {
    //pointer to hold the base address of the GPIO peripheral
    GPIO_RegDef_t *pGPIOx; // This holds the base address of the GPIO port to which the pin belongs
    // prefix p: pointer
    GPIO_PinConfig_t  GPIO_PinConfig;// This holds GPIO pin configuration setting

}GPIO_Handle_t;

/**
 *  @GPIO_PIN_NUMBERS
 *  GPIO pin numbers
 */
#define GPIO_PIN_NO_0       0
#define GPIO_PIN_NO_1       1
#define GPIO_PIN_NO_2       2
#define GPIO_PIN_NO_3       3
#define GPIO_PIN_NO_4       4
#define GPIO_PIN_NO_5       5
#define GPIO_PIN_NO_6       6
#define GPIO_PIN_NO_7       7
#define GPIO_PIN_NO_8       8
#define GPIO_PIN_NO_9       9
#define GPIO_PIN_NO_10      10
#define GPIO_PIN_NO_11      11
#define GPIO_PIN_NO_12      12
#define GPIO_PIN_NO_13      13
#define GPIO_PIN_NO_14      14
#define GPIO_PIN_NO_15      15
/**
 *  @GPIO_PIN_MODES
 *  GPIO ipn possible modes
 */
//non-interrupt mode (input mode)
#define GPIO_MODE_INPUT     0   
#define GPIO_MODE_OUTPUT    1
#define GPIO_MODE_ALTFN     2
#define GPIO_MODE_ANALOG    3
//interrupt mode (input mode)
#define GPIO_MODE_IT_FT     4 // interrupt falling edge trigger
#define GPIO_MODE_IT_RT     5 // interrupt rising edge
#define GPIO_MODE_IT_RFT    6 // interrupt rising falling edge trigger

/**
 * @GPIO_PIN_OPTYPE
 * GPIO pin possible output type ch6.4.2
*/
#define GPIO_OP_TYPE_PP     0 //pushpull
#define GPIO_OP_TYPE_OD     1 // open drain

/**
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds ch6.4.3
*/
#define GPIO_SPEED_LOW      0
#define GPIO_SPEED_MEDIUM   1
#define GPIO_SPEED_FAST     2
#define GPIO_SPEED_HIGH     3

/**
 * @GPIO_PIN_PPC>
 * GPIO pin pull up and pull down configuration macros ch6.4.4
*/

#define GPIO_NO_PUPD        0
#define GPIO_PIN_PU         1
#define GPIO_PIN_PD         2

/*******************************************************************************************
 *                          APIs supported by thie driver
 *          For more information about the APIs check the function definitions
 *******************************************************************************************/

/* 
 Peripheral Clock setup
*/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);// Given the GPIOx port base address, Enable or disable the clock



/*
 Init and De-int
*/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);
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
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
ISR handling
*/
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQCHandler(uint8_t PinNumber);



#endif /* INC_STM32F746XX_GPIO_H_ */
