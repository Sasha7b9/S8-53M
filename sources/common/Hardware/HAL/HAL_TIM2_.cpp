#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


void HAL_TIM2::Init()
{
    // Таймер для тиков
    TIM_HandleTypeDef handleTIM2 =
    {
        TIM2, //-V2571
        {
            0,
            TIM_COUNTERMODE_UP,
            0xffffffffU,
            TIM_CLOCKDIVISION_DIV1
        }
    };

    HAL_TIM_Base_Init(&handleTIM2);

    HAL_TIM_Base_Start(&handleTIM2);
}


uint HAL_TIM2::GetTicks()
{
#ifdef WIN32
    return TimeMS();
#else
    return TIM2->CNT; //-V2571
#endif
}


void HAL_TIM2::StartMultiMeasurement()
{
#ifndef WIN32
    TIM2->CR1 &= (uint)~TIM_CR1_CEN; //-V2571
    TIM2->CNT = 0; //-V2571
    TIM2->CR1 |= TIM_CR1_CEN; //-V2571
#endif
}


void HAL_TIM2::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}


uint HAL_TIM2::TimeMS()
{
    return HAL_GetTick();
}
