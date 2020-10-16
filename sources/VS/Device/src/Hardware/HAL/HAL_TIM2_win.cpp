#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <ctime>


uint HAL_TIM2::GetTicks()
{
    return static_cast<uint>(clock());
}


uint HAL_TIM2::TimeMS()
{
    return static_cast<uint>(clock());
}


void HAL_TIM2::StartMultiMeasurement()
{

}


void HAL_TIM2::Delay(uint)
{

}
