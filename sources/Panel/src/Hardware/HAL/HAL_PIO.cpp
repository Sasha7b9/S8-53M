#include "Hardware/HAL/HAL_PIO.h"
#include <stm32l0xx_hal.h>


#define SL0_Pin GPIO_PIN_0
#define SL0_GPIO_Port GPIOA
#define SL1_Pin GPIO_PIN_1
#define SL1_GPIO_Port GPIOA
#define SL2_Pin GPIO_PIN_2
#define SL2_GPIO_Port GPIOA
#define SL3_Pin GPIO_PIN_3
#define SL3_GPIO_Port GPIOA
#define SL4_Pin GPIO_PIN_4
#define SL4_GPIO_Port GPIOA
#define SL5_Pin GPIO_PIN_5
#define SL5_GPIO_Port GPIOA
#define RL0_Pin GPIO_PIN_0
#define RL0_GPIO_Port GPIOB
#define RL1_Pin GPIO_PIN_1
#define RL1_GPIO_Port GPIOB
#define RL2_Pin GPIO_PIN_2
#define RL2_GPIO_Port GPIOB
#define ON_Pin GPIO_PIN_8
#define ON_GPIO_Port GPIOA
#define RC2_Pin GPIO_PIN_9
#define RC2_GPIO_Port GPIOA
#define RC1_Pin GPIO_PIN_10
#define RC1_GPIO_Port GPIOA
#define RC0_Pin GPIO_PIN_11
#define RC0_GPIO_Port GPIOA
#define RC3_Pin GPIO_PIN_12
#define RC3_GPIO_Port GPIOA
#define RL3_Pin GPIO_PIN_3
#define RL3_GPIO_Port GPIOB
#define RL4_Pin GPIO_PIN_4
#define RL4_GPIO_Port GPIOB
#define RL5_Pin GPIO_PIN_5
#define RL5_GPIO_Port GPIOB
#define RL6_Pin GPIO_PIN_6
#define RL6_GPIO_Port GPIOB
#define RL7_Pin GPIO_PIN_7
#define RL7_GPIO_Port GPIOB


void HAL_PINS::Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, SL0_Pin | SL1_Pin | SL2_Pin | SL3_Pin
                      | SL4_Pin | SL5_Pin | ON_Pin | RC2_Pin
                      | RC1_Pin | RC0_Pin | RC3_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : SL0_Pin SL1_Pin SL2_Pin SL3_Pin
                             SL4_Pin SL5_Pin ON_Pin RC2_Pin
                             RC1_Pin RC0_Pin RC3_Pin */
    GPIO_InitStruct.Pin = SL0_Pin | SL1_Pin | SL2_Pin | SL3_Pin
        | SL4_Pin | SL5_Pin | ON_Pin | RC2_Pin
        | RC1_Pin | RC0_Pin | RC3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : RL0_Pin RL1_Pin RL2_Pin RL3_Pin
                             RL4_Pin RL5_Pin RL6_Pin RL7_Pin */
    GPIO_InitStruct.Pin = RL0_Pin | RL1_Pin | RL2_Pin | RL3_Pin
        | RL4_Pin | RL5_Pin | RL6_Pin | RL7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
