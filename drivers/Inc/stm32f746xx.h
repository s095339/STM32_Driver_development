/*
 * stm32f746xx.h
 *
 *  Created on: Jun 19, 2025
 *      Author: s0953
 */

#ifndef INC_STM32F746XX_H_
#define INC_STM32F746XX_H_


#include <stdint.h>
#include <stddef.h>
#include <string.h>
/************************　 Processor Specific Details *******************/
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
/*********************************************Micro Controller**********************************************/
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

//SPI 
typedef struct {
    volatile uint32_t CR1;
    // |15:BIDIMODE|14:BIDIOE|13:CRCEN|12:CRCNEXT|11:CRCL|10:RXONLY|9:SSM|8:SSI|
    // |7:LSBFIRST|6:SPE|5-3:BR[2:0]|2:MSTR|1:CPOL|0:CPHA|

    volatile uint32_t CR2;
    // |15-14:Reserved|13:LDMA_TX|12:LDMA_RX|11-10:FRXTH|9-8:DS[3:0]|7:TXEIE|
    // |6:RXNEIE|5:ERRIE|4:FRF|3:NSSP|2:SSOE|1:TXDMAEN|0:RXDMAEN|

    volatile uint32_t SR;
    // |15-11:Reserved|10:FTLVL[1:0]|8-9:FRLVL[1:0]|7:FRE|6:BSY|5:OVR|
    // |4:MODF|3:CRCERR|2:UDR|1:CHSIDE|0:RXNE|

    volatile uint32_t DR;
    // |15-0:DR[15:0]|

    volatile uint32_t CRCPR;
    // |15-0:CRCPOLY[15:0]|

    volatile uint32_t RXCRCR;
    // |15-0:RXCRC[15:0]|

    volatile uint32_t TXCRCR;
    // |15-0:TXCRC[15:0]|

    volatile uint32_t I2SCFGR;
    // |15:ASTRTEN|14:I2SMOD|13:I2SE|12:I2SCFG[1:0]|10:PCMSYNC|9:I2SSTD|
    // |8:CKPOL|7:DATLEN[1:0]|5:CHLEN|

    volatile uint32_t I2SPR;
    // |15-9:Reserved|8:MCKOE|7:ODD|6-0:I2SDIV[7:0]|

} SPI_RegDef_t;

//I2C
typedef struct {
    volatile uint32_t CR1;        // 0x00
    volatile uint32_t CR2;        // 0x04
    volatile uint32_t OAR1;       // 0x08
    volatile uint32_t OAR2;       // 0x0C
    volatile uint32_t TIMINGR;    // 0x10
    volatile uint32_t TIMEOUTR;   // 0x14
    volatile uint32_t ISR;        // 0x18
    volatile uint32_t ICR;        // 0x1C
    volatile uint32_t PECR;       // 0x20
    volatile uint32_t RXDR;       // 0x24
    volatile uint32_t TXDR;       // 0x28
}I2C_RegDef_t;


/****************************Peripheral register bit definition**************************/

/****************************
 *SPI and I2S bit definition*
 ****************************/
// SPIx CR1
enum {
    SPIx_CR1_CPHA     = 0,
    SPIx_CR1_CPOL     = 1,
    SPIx_CR1_MSTR     = 2,
    SPIx_CR1_BR       = 3,  // BR[2:0] from bit 3 to 5
    SPIx_CR1_SPE      = 6,
    SPIx_CR1_LSBFIRST = 7,
    SPIx_CR1_SSI      = 8,
    SPIx_CR1_SSM      = 9,
    SPIx_CR1_RXONLY   = 10,
    SPIx_CR1_CRCL     = 11,
    SPIx_CR1_CRCNEXT  = 12,
    SPIx_CR1_CRCEN    = 13,
    SPIx_CR1_BIDIOE   = 14,
    SPIx_CR1_BIDIMODE = 15
};
// SPIx_CR2
enum {
    SPIx_CR2_RXDMAEN  = 0,
    SPIx_CR2_TXDMAEN  = 1,
    SPIx_CR2_SSOE     = 2,
    SPIx_CR2_NSSP     = 3,
    SPIx_CR2_FRF      = 4,
    SPIx_CR2_ERRIE    = 5,
    SPIx_CR2_RXNEIE   = 6,
    SPIx_CR2_TXEIE    = 7,
    SPIx_CR2_DS       = 8,  // DS[3:0] 從 bit 8~11
    SPIx_CR2_FRXTH    = 12,
    SPIx_CR2_LDMA_RX  = 13,
    SPIx_CR2_LDMA_TX  = 14
};

//SPIx_SR
enum {
    SPIx_SR_RXNE      = 0,
    SPIx_SR_TXE       = 1,
    SPIx_SR_CHSIDE    = 2,
    SPIx_SR_UDR       = 3,
    SPIx_SR_CRCERR    = 4,
    SPIx_SR_MODF      = 5,
    SPIx_SR_OVR       = 6,
    SPIx_SR_BSY       = 7,
    SPIx_SR_FRE       = 8,
    SPIx_SR_FRLVL     = 9,  // FRLVL[1:0] 在 bit 9~10
    SPIx_SR_FTLVL     = 11  // FTLVL[1:0] 在 bit 11~12
};

/****************************
 *    I2C bit definition    *
 ****************************/
typedef enum {
    I2C_CR1_PE        = 0,
    I2C_CR1_TXIE      = 1,
    I2C_CR1_RXIE      = 2,
    I2C_CR1_ADDRIE    = 3,
    I2C_CR1_NACKIE    = 4,
    I2C_CR1_STOPIE    = 5,
    I2C_CR1_TCIE      = 6,
    I2C_CR1_ERRIE     = 7,
    I2C_CR1_DNF       = 8,   // DNF[3:0]
    I2C_CR1_ANFOFF    = 12,
    I2C_CR1_TXDMAEN   = 14,
    I2C_CR1_RXDMAEN   = 15,
    I2C_CR1_SBC       = 16,
    I2C_CR1_NOSTRETCH = 17,
    I2C_CR1_GCEN      = 19,
    I2C_CR1_SMBHEN    = 20,
    I2C_CR1_SMBDEN    = 21,
    I2C_CR1_ALERTEN   = 22,
    I2C_CR1_PECEN     = 23
} I2C_CR1_Bit_t;

typedef enum {
    I2C_CR2_SADD       = 0,   // SADD[9:0]
    I2C_CR2_RD_WRN     = 10,
    I2C_CR2_ADD10      = 11,
    I2C_CR2_HEAD10R    = 12,
    I2C_CR2_START      = 13,
    I2C_CR2_STOP       = 14,
    I2C_CR2_NACK       = 15,
    I2C_CR2_NBYTES     = 16,  // NBYTES[7:0]
    I2C_CR2_RELOAD     = 24,
    I2C_CR2_AUTOEND    = 25,
    I2C_CR2_PECBYTE    = 26
} I2C_CR2_Bit_t;

typedef enum {
    I2C_OAR1_OA1       = 0,   // OA1[9:0]
    I2C_OAR1_OA1MODE   = 10,
    I2C_OAR1_OA1EN     = 15
} I2C_OAR1_Bit_t;

typedef enum {
    I2C_OAR2_OA2         = 1,   // OA2[7:1]
    I2C_OAR2_OA2MSK      = 8,   // OA2MSK[2:0]
    I2C_OAR2_OA2EN       = 15
} I2C_OAR2_Bit_t;

typedef enum {
    I2C_TIMINGR_SCLL   = 0,
    I2C_TIMINGR_SCLH   = 8,
    I2C_TIMINGR_SDADEL = 16,
    I2C_TIMINGR_SCLDEL = 20,
    I2C_TIMINGR_PRESC  = 28
} I2C_TIMINGR_Bit_t;

typedef enum {
    I2C_TIMEOUTR_TIMEOUTA     = 0,
    I2C_TIMEOUTR_TIDLE        = 12,
    I2C_TIMEOUTR_TIMOUTEN     = 15,
    I2C_TIMEOUTR_TIMEOUTB     = 16,
    I2C_TIMEOUTR_TEXTEN       = 31
} I2C_TIMEOUTR_Bit_t;

typedef enum {
    I2C_ISR_TXE         = 0,
    I2C_ISR_TXIS        = 1,
    I2C_ISR_RXNE        = 2,
    I2C_ISR_ADDR        = 3,
    I2C_ISR_NACKF       = 4,
    I2C_ISR_STOPF       = 5,
    I2C_ISR_TC          = 6,
    I2C_ISR_TCR         = 7,
    I2C_ISR_BERR        = 8,
    I2C_ISR_ARLO        = 9,
    I2C_ISR_OVR         = 10,
    I2C_ISR_PECERR      = 11,
    I2C_ISR_TIMEOUT     = 12,
    I2C_ISR_ALERT       = 13,
    I2C_ISR_BUSY        = 15,
    I2C_ISR_DIR         = 16,
    I2C_ISR_ADDCODE     = 17  // ADDCODE[6:0]
} I2C_ISR_Bit_t;

typedef enum {
    I2C_ICR_ADDRCF   = 3,
    I2C_ICR_NACKCF   = 4,
    I2C_ICR_STOPCF   = 5,
    I2C_ICR_BERRCF   = 8,
    I2C_ICR_ARLOCF   = 9,
    I2C_ICR_OVRCF    = 10,
    I2C_ICR_PECCF    = 11,
    I2C_ICR_TIMOUTCF = 12,
    I2C_ICR_ALERTCF  = 13
} I2C_ICR_Bit_t;

/****************************
 *    bit definition    *
 ****************************/


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

#define SPI1        ((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2I2S2    ((SPI_RegDef_t *)SPI2_I2S2_BASEADDR)
#define SPI3I2S3    ((SPI_RegDef_t *)SPI3_I2S3_BASEADDR)
#define SPI4        ((SPI_RegDef_t *)SPI4_BASEADDR)
#define SPI5        ((SPI_RegDef_t *)SPI5_BASEADDR)
#define SPI6        ((SPI_RegDef_t *)SPI6_BASEADDR)


#define I2C1        ((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2        ((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3        ((I2C_RegDef_t *)I2C3_BASEADDR)
#define I2C4        ((I2C_RegDef_t *)I2C4_BASEADDR)

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
* Clock Disable Macros for peripheral
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
 Macros to reset peripherals
*/

//GPIO reset
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

//spi reset
#define SPI1_REG_RESET()   do{ (RCC->APB2RSTR |=(1<<12) ); (RCC->APB2RSTR &= ~(1<<12) ); }while(0)
#define SPI4_REG_RESET()   do{ (RCC->APB2RSTR |=(1<<13) ); (RCC->APB2RSTR &= ~(1<<13) ); }while(0)
#define SPI5_REG_RESET()   do{ (RCC->APB2RSTR |=(1<<20) ); (RCC->APB2RSTR &= ~(1<<20) ); }while(0)
#define SPI6_REG_RESET()   do{ (RCC->APB2RSTR |=(1<<21) ); (RCC->APB2RSTR &= ~(1<<21) ); }while(0)

#define SPI2_REG_RESET()   do{ (RCC->APB1RSTR |=(1<<14) ); (RCC->APB1RSTR &= ~(1<<14) ); }while(0)
#define SPI3_REG_RESET()   do{ (RCC->APB1RSTR |=(1<<15) ); (RCC->APB1RSTR &= ~(1<<15) ); }while(0)

//i2c reset
#define I2C1_REG_RESET()   do{ (RCC->APB1RSTR |=(1<<21) ); (RCC->APB1RSTR &= ~(1<<21) ); }while(0)
#define I2C2_REG_RESET()   do{ (RCC->APB1RSTR |=(1<<22) ); (RCC->APB1RSTR &= ~(1<<22) ); }while(0)
#define I2C3_REG_RESET()   do{ (RCC->APB1RSTR |=(1<<23) ); (RCC->APB1RSTR &= ~(1<<23) ); }while(0)
#define I2C4_REG_RESET()   do{ (RCC->APB1RSTR |=(1<<24) ); (RCC->APB1RSTR &= ~(1<<24) ); }while(0)

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
#define FLAG_RESET          RESET
#define FLAG_SET            SET



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

// SPI
#define IRQ_NO_SPI1         35
#define IRQ_NO_SPI2         36
#define IRQ_NO_SPI3         51
#define IRQ_NO_SPI4         84  
#define IRQ_NO_SPI5         85
#define IRQ_NO_SPI6         86

// I2C
#define IRQ_NO_I2C1_EV      31 //event interrupt
#define IRQ_NO_I2C1_ER      32 //error interrupt
#define IRQ_NO_I2C2_EV      33 
#define IRQ_NO_I2C2_ER      34 
#define IRQ_NO_I2C3_EV      72 
#define IRQ_NO_I2C3_ER      73 
#define IRQ_NO_I2C4_EV      95 
#define IRQ_NO_I2C4_ER      96 

//Macros for all possible Interrupt priority
#define NVIC_IRQ_RRI0       0
#define NVIC_IRQ_RRI15      15
                             //include


//Clock
#define HSI_CLK_FREQ        16000000 //16M

#include "stm32f746xx_gpio.h"
#include "stm32f746xx_spi.h"
#include "stm32f746xx_i2c.h"
/* INC_STM32F746XX_H_ */
#endif 