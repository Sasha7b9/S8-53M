#include "main.h"
#include "stm32l0xx_it.h"
#include <stm32l0xx_hal.h>


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
