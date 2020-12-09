#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <stm32f4xx_hal.h>


static CRC_HandleTypeDef handleCRC = { CRC }; //-V2571


void HAL_CRC::Init()
{
    HAL_CRC_Init(&handleCRC);
}


uint HAL_CRC::Calculate(uint address, uint numBytes)
{
    return HAL_CRC_Calculate(&handleCRC, reinterpret_cast<uint *>(address), numBytes); //-V2571
}
