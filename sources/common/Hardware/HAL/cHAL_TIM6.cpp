#include "defines.h"
#include "Hardware/Timer.h"
#include "common/Hardware/HAL/HAL.h"
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


void HAL_TIM6::Init()
{
    // ������ ��� ��
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    HAL_TIM_Base_Init(&handleTIM6);

    HAL_TIM_Base_Start_IT(&handleTIM6);
}


#ifdef __cplusplus
extern "C" {
#endif


    void TIM6_DAC_IRQHandler(void)
    {
        if (__HAL_TIM_GET_FLAG(&handleTIM6, TIM_FLAG_UPDATE) == SET && __HAL_TIM_GET_ITSTATUS(&handleTIM6, TIM_IT_UPDATE)) //-V2570
        {
            Timer::Update1ms();
            __HAL_TIM_CLEAR_FLAG(&handleTIM6, TIM_FLAG_UPDATE);
            __HAL_TIM_CLEAR_IT(&handleTIM6, TIM_IT_UPDATE);
        }
    }

#ifdef __cplusplus
}
#endif
