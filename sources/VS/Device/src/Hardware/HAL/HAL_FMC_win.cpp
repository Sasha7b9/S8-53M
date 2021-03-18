// 2021/03/17 22:23:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"


static uint8 buffer[1024 * 1024];

static uint16 addr_fpga;

uint16 *const HAL_FMC::ADDR_FPGA = &addr_fpga;
uint16 *const HAL_FMC::ADDR_RAM_BEGIN = (uint16 *)&buffer[0];


void HAL_FMC::Init()
{

}


void HAL_FMC::Write(uint16 *const, uint16)
{

}
