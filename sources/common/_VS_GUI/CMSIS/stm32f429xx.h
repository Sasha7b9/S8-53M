// 2021/06/20 19:06:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "system_stm32f4xx.h"


typedef enum
{
    DISABLE = 0U,
    ENABLE = !DISABLE
} FunctionalState;


#define ADC_RESOLUTION12b                0
#define ADC_CLOCKPRESCALER_PCLK_DIV2     0
#define ADC_DATAALIGN_RIGHT              0
#define ADC_EXTERNALTRIGCONVEDGE_NONE    0
#define ADC_EXTERNALTRIGINJECCONV_T1_CC4 0
#define ADC_CHANNEL_0                    0
#define ADC_SAMPLETIME_28CYCLES          0
#define ADC_CR2_ADON                     0


#define ADC1                ((ADC_TypeDef *) 0)


#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


typedef struct
{
    __IO uint32_t BTCR[8];    /*!< NOR/PSRAM chip-select control register(BCR) and chip-select timing register(BTR), Address offset: 0x00-1C */
} FMC_Bank1_TypeDef;


typedef enum
{
    EXTI4_IRQn = 10,     /*!< EXTI Line4 Interrupt                                              */
    ADC_IRQn = 18     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
} IRQn_Type;


typedef struct
{
    __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;


typedef struct
{
    __IO uint32_t SR;     /*!< ADC status register,                         Address offset: 0x00 */
    __IO uint32_t CR1;    /*!< ADC control register 1,                      Address offset: 0x04 */
    __IO uint32_t CR2;    /*!< ADC control register 2,                      Address offset: 0x08 */
    __IO uint32_t SMPR1;  /*!< ADC sample time register 1,                  Address offset: 0x0C */
    __IO uint32_t SMPR2;  /*!< ADC sample time register 2,                  Address offset: 0x10 */
    __IO uint32_t JOFR1;  /*!< ADC injected channel data offset register 1, Address offset: 0x14 */
    __IO uint32_t JOFR2;  /*!< ADC injected channel data offset register 2, Address offset: 0x18 */
    __IO uint32_t JOFR3;  /*!< ADC injected channel data offset register 3, Address offset: 0x1C */
    __IO uint32_t JOFR4;  /*!< ADC injected channel data offset register 4, Address offset: 0x20 */
    __IO uint32_t HTR;    /*!< ADC watchdog higher threshold register,      Address offset: 0x24 */
    __IO uint32_t LTR;    /*!< ADC watchdog lower threshold register,       Address offset: 0x28 */
    __IO uint32_t SQR1;   /*!< ADC regular sequence register 1,             Address offset: 0x2C */
    __IO uint32_t SQR2;   /*!< ADC regular sequence register 2,             Address offset: 0x30 */
    __IO uint32_t SQR3;   /*!< ADC regular sequence register 3,             Address offset: 0x34 */
    __IO uint32_t JSQR;   /*!< ADC injected sequence register,              Address offset: 0x38*/
    __IO uint32_t JDR1;   /*!< ADC injected data register 1,                Address offset: 0x3C */
    __IO uint32_t JDR2;   /*!< ADC injected data register 2,                Address offset: 0x40 */
    __IO uint32_t JDR3;   /*!< ADC injected data register 3,                Address offset: 0x44 */
    __IO uint32_t JDR4;   /*!< ADC injected data register 4,                Address offset: 0x48 */
    __IO uint32_t DR;     /*!< ADC regular data register,                   Address offset: 0x4C */
} ADC_TypeDef;
