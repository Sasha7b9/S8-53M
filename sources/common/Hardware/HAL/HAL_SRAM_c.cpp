#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <stm32f4xx_hal.h>


void HAL_SRAM::Init()
{
    //    __FSMC_CLK_ENABLE();
//    __GPIOB_CLK_ENABLE();
//    __GPIOD_CLK_ENABLE();
//    __GPIOE_CLK_ENABLE();
//
//    GPIO_InitTypeDef isGPIOD = {
//        GPIO_PIN_0 |        // DA2
//        GPIO_PIN_1 |        // DA3
//        GPIO_PIN_4 |        // RD
//        GPIO_PIN_5 |        // WR
//        GPIO_PIN_7 |        // NE1
//        GPIO_PIN_11 |       // A16
//        GPIO_PIN_12 |       // A17
//        GPIO_PIN_13 |       // A18
//        GPIO_PIN_14 |       // DA0
//        GPIO_PIN_15,        // DA1
//        GPIO_MODE_AF_PP,
//        GPIO_NOPULL,
//        GPIO_SPEED_FAST,
//        GPIO_AF12_FSMC
//    };
//    HAL_GPIO_Init(GPIOD, &isGPIOD);
//
//    GPIO_InitTypeDef isGPIOE_B = {
//        GPIO_PIN_2 |        // A23
//        GPIO_PIN_3 |        // A19
//        GPIO_PIN_4 |        // A20
//        GPIO_PIN_5 |        // A21
//        GPIO_PIN_6 |        // A22
//        GPIO_PIN_7 |        // DA4
//        GPIO_PIN_8 |        // DA5
//        GPIO_PIN_9 |        // DA6
//        GPIO_PIN_10,        // DA7
//        GPIO_MODE_AF_PP,
//        GPIO_NOPULL,
//        GPIO_SPEED_FAST,
//        GPIO_AF12_FSMC
//    };
//    HAL_GPIO_Init(GPIOE, &isGPIOE_B);
//
//    isGPIOE_B.Pin = GPIO_PIN_7;     // NL1
//    HAL_GPIO_Init(GPIOB, &isGPIOE_B);
}
