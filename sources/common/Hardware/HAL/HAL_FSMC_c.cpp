#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "common/Hardware/Timer_c.h"
#include <stm32f4xx_hal.h>


static const uint ADDR_BANK = 0x60000000;


uint8 * const HAL_FSMC::ADDR_FPGA = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00c80000));  // Адрес записи в аппаратные регистры. //-V566 //-V2571
uint8 * const HAL_FSMC::ADDR_NULL = (reinterpret_cast<uint8 *>(ADDR_BANK + 0x00a00000)); //-V566 //-V2571


uint8 HAL_FSMC::Read(pUCHAR const address)
{
    return(*(address));
}


void HAL_FSMC::Write(uint8 * const address, uint8 value)
{
    *address = value;
}


void HAL_FSMC::Init()
{

}
