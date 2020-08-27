#include "main.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


SPI_HandleTypeDef hspi2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    HAL::Init();

    MX_GPIO_Init();

    while (1)
    {
    }
}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}


static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

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


void Error_Handler(void)
{
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
