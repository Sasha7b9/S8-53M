#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>

void Timer::PauseOnTime(uint timeMS)
{
    HAL_Delay(timeMS);
}
