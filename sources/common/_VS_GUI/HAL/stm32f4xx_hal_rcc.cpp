#include "defines.h"
#include <stm32f4xx_hal.h>


__weak HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *)
{
    return HAL_ERROR;
}


HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *, uint32_t)
{
    return HAL_ERROR;
}


void HAL_RCC_MCOConfig(uint32_t, uint32_t, uint32_t)
{

}


HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *)
{
    return HAL_ERROR;
}
