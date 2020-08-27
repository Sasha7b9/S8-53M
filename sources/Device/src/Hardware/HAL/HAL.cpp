#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


void HAL::Init()
{
    HAL_Init();

    __ADC3_CLK_ENABLE();

    HAL_PINS::Init();
}
