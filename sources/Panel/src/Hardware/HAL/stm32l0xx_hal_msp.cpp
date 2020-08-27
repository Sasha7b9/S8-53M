#include "main.h"
#include <stm32l0xx_hal.h>


void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{

}

