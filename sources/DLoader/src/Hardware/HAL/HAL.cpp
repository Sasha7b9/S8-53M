#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


void HAL::Init()
{

}


uint HAL_TIM::GetTimeMS()
{
    return HAL_GetTick();
}
