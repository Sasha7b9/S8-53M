// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef handleTIM6 =
{
    TIM6,
    {
        119,                    // Init.Prescaler
        TIM_COUNTERMODE_UP,     // Init.CounterMode
        500,                    // Init.Period
        TIM_CLOCKDIVISION_DIV1  // Init.ClockDivision
    }
};


void *HAL_TIM6::handle = &handleTIM6;


void HAL_TIM6::Init()
{
    // Таймер для мс
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    HAL_TIM_Base_Init(&handleTIM6);

    HAL_TIM_Base_Start_IT(&handleTIM6);
}
