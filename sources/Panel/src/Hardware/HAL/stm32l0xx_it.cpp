#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "stm32l0xx_it.h"
#include <stm32l0xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif


void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
    while (1)
    {
    }
}


void SVC_Handler(void)
{

}


void PendSV_Handler(void)
{

}


void SysTick_Handler(void)
{
    HAL_IncTick();
}


void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(reinterpret_cast<TIM_HandleTypeDef *>(HAL_TIM2::handle));
}


#ifdef __cplusplus
}
#endif
