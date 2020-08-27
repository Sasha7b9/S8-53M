#include "main.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


SPI_HandleTypeDef hspi2;


static void MX_GPIO_Init(void);

int main(void)
{
    HAL_Init();

    HAL::Init();

    MX_GPIO_Init();

    while (1)
    {
    }
}


static void MX_GPIO_Init(void)
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


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
