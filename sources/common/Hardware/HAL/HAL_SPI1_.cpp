// 2021/02/26 9:52:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


void HAL_SPI1::Init()
{
    HAL_PINS::SPI1_::Init();
}
