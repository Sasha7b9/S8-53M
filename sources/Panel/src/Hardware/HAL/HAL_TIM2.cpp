#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


#define TIME_UPDATE 2   // Время между опросами клавиатуры


static TIM_HandleTypeDef handleTIM2 =
{
    TIM2,
    {
        static_cast<uint>((SystemCoreClock / 2) / 10000) - 1,
        TIM_COUNTERMODE_UP,
        TIME_UPDATE * 10 - 1,
        0
    }
};


void *HAL_TIM2::handle = &handleTIM2;

static void (*callbackKeyboard)() = nullptr;


void HAL_TIM2::Init(void (*func)())
{
    callbackKeyboard = func;

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    HAL_TIM_Base_Init(&handleTIM2);

    HAL_TIM_Base_Start_IT(&handleTIM2);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *)
{
    callbackKeyboard();
}
