#include "defines.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/HAL/HAL_PINS_.h"
#include "Hardware/Hardware.h"
#include "Panel/Panel.h"



void Hardware_Init() 
{
    HAL::Init();
}
