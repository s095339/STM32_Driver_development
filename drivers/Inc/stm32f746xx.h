/*
 * stm32f746xx.h
 *
 *  Created on: Jun 19, 2025
 *      Author: s0953
 */

#ifndef INC_STM32F746XX_H_
#define INC_STM32F746XX_H_


#include <stdint.h>

/************************START: Processor Specific Details *******************/
//ARM® Cortex®-M7 Devices Generic user guide ch4.2 

//interrupt Set-enable Registers
#define NVIC_ISER0   (volatile uint32_t*)0xE000E100
#define NVIC_ISER1   (volatile uint32_t*)0xE000E104
#define NVIC_ISER2   (volatile uint32_t*)0xE000E108
#define NVIC_ISER3   (volatile uint32_t*)0xE000E10C
//下面用不到
//#define NVIC_ISER0   (volatile uint32_t*)0xE000E100
//#define NVIC_ISER0   (volatile uint32_t*)0xE000E100
//#define NVIC_ISER0   (volatile uint32_t*)0xE000E100
//#define NVIC_ISER0   (volatile uint32_t*)0xE000E100


//interrupt Clear-enable Register
#define NVIC_ICER0              (volatile uint32_t*)0XE000E180
#define NVIC_ICER1              (volatile uint32_t*)0xE000E184
#define NVIC_ICER2              (volatile uint32_t*)0xE000E188
#define NVIC_ICER3              (volatile uint32_t*)0xE000E18C


// Interrupt Priority Registers
#define NVIC_PR_BASE_ADDR       (volatile uint32_t*)0xE000E400

#define NO_PR_BITS_IMPLEMENTED  4 
//根據UM 10.1 NVIC features 
//雖然cortex-M7的processor提供了8bits的中段優先級，但是STM32的這個microprocessor只implement了4bits
/*****************************************************8 *******************/
/*
* Base addresses of Glash and SRAM memories based on User Manual
*/
#define FLASH_BASEADDR          0x80000000UL // Flash Memory (CH3)
#define SRAM1_BASEADDR          0x20010000UL // (CH2)
#define SRAM2_BASEADDR          0x2004C000UL // (CH1)
#define ROM_BASEADDR            0x1FF00000UL             //system memory Usermanul p.79
#define SRAM                    SRAM1_BASEADDR


/// BUS ///

/*
* AHBx and APBx Bus Peripheral base addresses
*/
#define PERIPH_BASEADDR         0x40000000UL
#define APB1PERIPH_BASEADDR     PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR     0x40010000UL
#define AHB1PERIPH_BASEADDR     0x40020000UL
#define AHB2PERIPH_BASEADDR     0x50000000UL
#define AHB3PERIPH_BASEADDR     0x60000000UL



/*
*   Base address of peripherals which are hanging on AHB1 bus
*   (只放我們會用到的)
*/
#define GPIOA_BASEADDR          (AHB1PERIPH_BASEADDR + 0x0000UL)
#define GPIOB_BASEADDR          (AHB1PERIPH_BASEADDR + 0x0400UL)
#define GPIOC_BASEADDR          (AHB1PERIPH_BASEADDR + 0x0800UL)
#define GPIOD_BASEADDR          (AHB1PERIPH_BASEADDR + 0x0C00UL)
#define GPIOE_BASEADDR          (AHB1PERIPH_BASEADDR + 0x1000UL)
#define GPIOF_BASEADDR          (AHB1PERIPH_BASEADDR + 0x1400UL)
#define GPIOG_BASEADDR          (AHB1PERIPH_BASEADDR + 0x1800UL)
#define GPIOH_BASEADDR          (AHB1PERIPH_BASEADDR + 0x1C00UL)
#define GPIOI_BASEADDR          (AHB1PERIPH_BASEADDR + 0x2000UL)
#define GPIOJ_BASEADDR          (AHB1PERIPH_BASEADDR + 0x2400UL)
#define GPIOK_BASEADDR          (AHB1PERIPH_BASEADDR + 0x2800UL)

#define CRC_BASEADDR            (AHB1PERIPH_BASEADDR + 0x3000UL)
#define RCC_BASEADDR            (AHB1PERIPH_BASEADDR + 0x3800UL)
/*
*   Base address of peripherals which are hanging on APB1 bus
*   Timer, SPI, I2C, UART,USART
*   (只放我們會用到的)
*/
#define TIM2_BASEADDR           (APB1PERIPH_BASEADDR + 0x0000UL)
#define TIM3_BASEADDR           (APB1PERIPH_BASEADDR + 0x0400UL)
#define TIM4_BASEADDR           (APB1PERIPH_BASEADDR + 0x0800UL)
#define TIM5_BASEADDR           (APB1PERIPH_BASEADDR + 0x0C00UL)
#define TIM6_BASEADDR           (APB1PERIPH_BASEADDR + 0x1000UL)
#define TIM7_BASEADDR           (APB1PERIPH_BASEADDR + 0x1400UL)
#define TIM12_BASEADDR          (APB1PERIPH_BASEADDR + 0x1800UL)
#define TIM13_BASEADDR          (APB1PERIPH_BASEADDR + 0x1C00UL)
#define TIM14_BASEADDR          (APB1PERIPH_BASEADDR + 0x2000UL)

#define SPI2_I2S2_BASEADDR      (APB1PERIPH_BASEADDR + 0x3800UL)
#define SPI3_I2S3_BASEADDR      (APB1PERIPH_BASEADDR + 0x3C00UL)

#define USART2_BASEADDR         (APB1PERIPH_BASEADDR + 0x4400UL)
#define USART3_BASEADDR         (APB1PERIPH_BASEADDR + 0x4800UL)
#define UART4_BASEADDR          (APB1PERIPH_BASEADDR + 0x4C00UL)
#define UART5_BASEADDR          (APB1PERIPH_BASEADDR + 0x5000UL)

#define I2C1_BASEADDR           (APB1PERIPH_BASEADDR + 0x5400UL)
#define I2C2_BASEADDR           (APB1PERIPH_BASEADDR + 0x5800UL)
#define I2C3_BASEADDR           (APB1PERIPH_BASEADDR + 0x5C00UL)
#define I2C4_BASEADDR           (APB1PERIPH_BASEADDR + 0x6000UL)

#define UART8_BASEADDR          (APB1PERIPH_BASEADDR + 0x7C00UL)

/*
*   Base address of peripherals which are hanging on APB2 bus
*   SPI USAR Timer EXTI LCD-TFT
*   (只放我們會用到的)
*/

#define TIM1_BASEADDR           (APB2PERIPH_BASEADDR + 0x0000UL)
#define TIM8_BASEADDR           (APB2PERIPH_BASEADDR + 0x0400UL)
#define USART1_BASEADDR         (APB2PERIPH_BASEADDR + 0x1000UL)
#define USART6_BASEADDR         (APB2PERIPH_BASEADDR + 0x1400UL)

#define SPI1_BASEADDR           (APB2PERIPH_BASEADDR + 0x3000UL)
#define SPI4_BASEADDR           (APB2PERIPH_BASEADDR + 0x3400UL)
#define SYSCFG_BASEADDR         (APB2PERIPH_BASEADDR + 0x3800UL)
#define EXTI_BASEADDR           (APB2PERIPH_BASEADDR + 0x3C00UL)

#define TIM9_BASEADDR           (APB2PERIPH_BASEADDR + 0x4000UL)
#define TIM10_BASEADDR          (APB2PERIPH_BASEADDR + 0x4400UL)
#define TIM11_BASEADDR          (APB2PERIPH_BASEADDR + 0x4800UL)

#define SPI5_BASEADDR           (APB2PERIPH_BASEADDR + 0x5000UL)
#define SPI6_BASEADDR           (APB2PERIPH_BASEADDR + 0x5400UL)

#define LCD_TFT_BASEADDR        (APB2PERIPH_BASEADDR + 0x6800UL)


/****************************Peripheral register definition structures**************************/

// GPIO
// Refer to the User manual 6.4.11 GPIO register map
// This structure is a general structure for all GPIO


//structure的空間會是連續的 所以MODER如果在0x00那麼OTYPER就會在0x04 (32bits=4bytes 四個位置)
typedef struct{
    volatile uint32_t MODER;                         /*!< GPIO Port mode register                                                                  Address offset:0x00>*/
    volatile uint32_t OTYPER;                        /*!< GPIO Port output type register                                                           Address offset:0x04>*/
    volatile uint32_t OSPEEDR;                       /*!< GPIO Port output speed register                                                          Address offset:0x08>*/
    volatile uint32_t PUPDR;                         /*!< GPIO Port pull-up/pull-down register                                                     Address offset:0x0C>*/
    volatile uint32_t IDR;                           /*!< GPIO Port input data register                                                            Address offset:0x10>*/
    volatile uint32_t ODR;                           /*!< GPIO Port output data register                                                           Address offset:0x14>*/
    volatile uint32_t BSRR;                          /*!< GPIO Port bit set/reset register                                                         Address offset:0x18>*/
    volatile uint32_t LCKR;                          /*!< GPIO Port configuration lock register register                                           Address offset:0x1C>*/
    volatile uint32_t AFR[2];                        /*!< [0]GPIO alternate function low register,[1]GPIO alternate function HIGH register         Address offset:0x20>*/
}GPIO_RegDef_t;

//RCC
// CH 5.3.27
typedef struct 
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t AHB3LPENR;
    volatile uint32_t RESERVED4;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t RESERVED5[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t PLLSAICFGR;
    volatile uint32_t DCKCFGR1;
    volatile uint32_t DCKCFGR2;

}RCC_RegDef_t;


//EXTI
//11.9.7

typedef struct{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
}EXTI_RegDef_t;

//SYSSCFG
typedef struct{
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    volatile uint32_t RESERVE[2];
    volatile uint32_t CMPCR;
}SYSCFG_RegDef_t;

/****************************Peripheral definitions **************************/


#define GPIOA  ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB  ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC  ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD  ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE  ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF  ((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG  ((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH  ((GPIO_RegDef_t *)GPIOH_BASEADDR)
#define GPIOI  ((GPIO_RegDef_t *)GPIOI_BASEADDR)
#define GPIOJ  ((GPIO_RegDef_t *)GPIOJ_BASEADDR)
#define GPIOK  ((GPIO_RegDef_t *)GPIOK_BASEADDR)

#define RCC    ((RCC_RegDef_t *)RCC_BASEADDR)

#define EXTI   ((EXTI_RegDef_t *)EXTI_BASEADDR)

#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)


/*
* Clock Enable Macros for GPIOx peripherals
*/
// Since gpiox are on the AHB1 bus, so we need to access the RCC AHB1 peripheral clock register ch5.3.10
// PLCK = peripheral clock
#define GPIOA_PLCK_EN()   (RCC->AHB1ENR |=(1<<0))  
#define GPIOB_PLCK_EN()   (RCC->AHB1ENR |=(1<<1)) 
#define GPIOC_PLCK_EN()   (RCC->AHB1ENR |=(1<<2)) 
#define GPIOD_PLCK_EN()   (RCC->AHB1ENR |=(1<<3)) 
#define GPIOE_PLCK_EN()   (RCC->AHB1ENR |=(1<<4)) 
#define GPIOF_PLCK_EN()   (RCC->AHB1ENR |=(1<<5)) 
#define GPIOG_PLCK_EN()   (RCC->AHB1ENR |=(1<<6)) 
#define GPIOH_PLCK_EN()   (RCC->AHB1ENR |=(1<<7)) 
#define GPIOI_PLCK_EN()   (RCC->AHB1ENR |=(1<<8)) 
#define GPIOJ_PLCK_EN()   (RCC->AHB1ENR |=(1<<9)) 
#define GPIOK_PLCK_EN()   (RCC->AHB1ENR |=(1<<10)) 

/*
* Clock Enable Macros for I2Cx peripherals
*/
#define I2C1_PLCK_EN()    (RCC->APB1ENR |=(1<<21))
#define I2C2_PLCK_EN()    (RCC->APB1ENR |=(1<<22))
#define I2C3_PLCK_EN()    (RCC->APB1ENR |=(1<<23))
#define I2C4_PLCK_EN()    (RCC->APB1ENR |=(1<<24))

/*
* Clock Enable Macros for SPIx peripherals
*/
#define SPI1_PLCK_EN()    (RCC->APB2ENR |=(1<<12))
#define SPI4_PLCK_EN()    (RCC->APB2ENR |=(1<<13))
#define SPI5_PLCK_EN()    (RCC->APB2ENR |=(1<<20))
#define SPI6_PLCK_EN()    (RCC->APB2ENR |=(1<<21))

#define SPI2_I2S2_PLCK_EN()    (RCC->APB1ENR |=(1<<14))
#define SPI3_I2S3_PLCK_EN()    (RCC->APB1ENR |=(1<<15))
/*
* Clock Enable Macros for USARTx peripherals
*/

#define USART1_PLCK_EN()  (RCC->APB2ENR |=(1<<4))
#define USART6_PLCK_EN()  (RCC->APB2ENR |=(1<<5))

#define USART2_PLCK_EN()  (RCC->APB1ENR |=(1<<17))
#define USART3_PLCK_EN()  (RCC->APB1ENR |=(1<<18))

#define UART4_PLCK_EN()  (RCC->APB1ENR |=(1<<19))
#define UART5_PLCK_EN()  (RCC->APB1ENR |=(1<<20))
#define UART7_PLCK_EN()  (RCC->APB1ENR |=(1<<30))
#define UART8_PLCK_EN()  (RCC->APB1ENR |=(1<<31))
/*
* Clock Enable Macros for SYSCFG peripheral
*/
#define SYSCFG_PLCK_EN() (RCC->APB2ENR |=(1<<14))

//////////



/*
* Clock Disable Macros for GPIOx peripherals
*/

#define GPIOA_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<0))  
#define GPIOB_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<1)) 
#define GPIOC_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<2)) 
#define GPIOD_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<3)) 
#define GPIOE_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<4)) 
#define GPIOF_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<5)) 
#define GPIOG_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<6)) 
#define GPIOH_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<7)) 
#define GPIOI_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<8)) 
#define GPIOJ_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<9)) 
#define GPIOK_PLCK_DI()   (RCC->AHB1ENR &= ~(1<<10)) 



/*
* Clock Disable Macros for I2Cx peripherals
*/
#define I2C1_PLCK_DI()    (RCC->APB1ENR &= ~(1<<21))
#define I2C2_PLCK_DI()    (RCC->APB1ENR &= ~(1<<22))
#define I2C3_PLCK_DI()    (RCC->APB1ENR &= ~(1<<23))
#define I2C4_PLCK_DI()    (RCC->APB1ENR &= ~(1<<24))
/*
* Clock Disable Macros for SPIx peripherals
*/
#define SPI1_PLCK_DI()    (RCC->APB2ENR &= ~(1<<12))
#define SPI4_PLCK_DI()    (RCC->APB2ENR &= ~(1<<13))
#define SPI5_PLCK_DI()    (RCC->APB2ENR &= ~(1<<20))
#define SPI6_PLCK_DI()    (RCC->APB2ENR &= ~(1<<21))

#define SPI2_I2S2_PLCK_DI()    (RCC->APB1ENR &= ~(1<<14))
#define SPI3_I2S3_PLCK_DI()    (RCC->APB1ENR &= ~(1<<15))
/*
* Clock Disable Macros for USARTx peripherals
*/

#define USART1_PLCK_DI()  (RCC->APB2ENR &= ~(1<<4))
#define USART6_PLCK_DI()  (RCC->APB2ENR &= ~(1<<5))

#define USART2_PLCK_DI()  (RCC->APB1ENR &= ~(1<<17))
#define USART3_PLCK_DI()  (RCC->APB1ENR &= ~(1<<18))

#define UART4_PLCK_DI()  (RCC->APB1ENR &= ~(1<<19))
#define UART5_PLCK_DI()  (RCC->APB1ENR &= ~(1<<20))
#define UART7_PLCK_DI()  (RCC->APB1ENR &= ~(1<<30))
#define UART8_PLCK_DI()  (RCC->APB1ENR &= ~(1<<31))

/*
* Clock Disable Macros for SYSCFG peripheral
*/
#define SYSCFG_PLCK_DI() (RCC->APB2ENR &= ~(1<<14))


/*
 Macros to reset GPIOx peripherals
*/

#define GPIOA_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<0)); (RCC->AHB1RSTR &= ~(1<<0));}while(0) //這樣就可以做兩件事情 做一次 因為while(0)
#define GPIOB_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<1)); (RCC->AHB1RSTR &= ~(1<<1));}while(0) 
#define GPIOC_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<2)); (RCC->AHB1RSTR &= ~(1<<2));}while(0) 
#define GPIOD_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<3)); (RCC->AHB1RSTR &= ~(1<<3));}while(0) 
#define GPIOE_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<4)); (RCC->AHB1RSTR &= ~(1<<4));}while(0) 
#define GPIOF_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<5)); (RCC->AHB1RSTR &= ~(1<<5));}while(0) 
#define GPIOG_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<6)); (RCC->AHB1RSTR &= ~(1<<6));}while(0) 
#define GPIOH_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<7)); (RCC->AHB1RSTR &= ~(1<<7));}while(0) 
#define GPIOI_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<8)); (RCC->AHB1RSTR &= ~(1<<8));}while(0) 
#define GPIOJ_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<9)); (RCC->AHB1RSTR &= ~(1<<9));}while(0) 
#define GPIOK_REG_RESET()  do{ (RCC->AHB1RSTR |=(1<<10)); (RCC->AHB1RSTR &= ~(1<<10));}while(0) 

//GPIO useful macro
//
#define GPIO_BASEADDR_TO_CODE(x)    ((x==GPIOA)?0:\
                                    (x==GPIOB)?1:\
                                    (x==GPIOC)?2:\
                                    (x==GPIOD)?3:\
                                    (x==GPIOE)?4:\
                                    (x==GPIOF)?5:\
                                    (x==GPIOG)?6:\
                                    (x==GPIOH)?7:\
                                    (x==GPIOI)?8:\
                                    (x==GPIOJ)?9:\
                                    (x==GPIOK)?10:0)


//some generic marcos

#define ENABLE              1
#define DISABLE             0
#define SET                 ENABLE
#define RESET               DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET




/****
 * IRQ(Interrupt Request) Number of STM32F746x MCU
 * NOTE: update these macros with valid values according to MCU
 * Vector table: 10.1.2
 */

// EXTI0~15
#define IRQ_NO_EXTI0        6
#define IRQ_NO_EXTI1        7
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5      23
#define IRQ_NO_EXTI15_10    40

//Macros for all possible Interrupt priority
#define NVIC_IRQ_RRI0       0
#define NVIC_IRQ_RRI15      15
                             //include
#include "stm32f746xx_gpio.h"


/* INC_STM32F746XX_H_ */
#endif 