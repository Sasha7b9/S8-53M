#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


uint HAL_TIM2::GetTicks()
{
    return TIM2->CNT;
}


void HAL_TIM2::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}


void HAL_TIM2::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}


uint HAL_TIM2::TimeMS()
{
    return HAL_GetTick();
}
