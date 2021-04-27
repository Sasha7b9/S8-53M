// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static CRC_HandleTypeDef handleCRC = { CRC };


void HAL_CRC::Init()
{
    HAL_CRC_Init(&handleCRC);
}


uint HAL_CRC::Calculate(uint address, uint numBytes)
{
    return HAL_CRC_Calculate(&handleCRC, reinterpret_cast<uint *>(address), numBytes);
}


uint HAL_CRC::Calculate8bit(uint8 *buffer, int size)
{
    if ((size % 4) != 0)
    {
        size -= (size % 4);
    }

    return HAL_CRC_Calculate(&handleCRC, reinterpret_cast<uint *>(buffer), static_cast<uint>(size / 4));
}
