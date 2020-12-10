#include "defines.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "common/Hardware/HAL/HAL_PINS_c.h"
#include "Hardware/Hardware.h"
#include "Panel/Panel.h"



void Hardware_Init() 
{
    HAL::Init();
}
