#include "Hardware.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include "common/Hardware/HAL/HAL.h"



void Hardware_Init() 
{
    HAL::Init();

    pinG1.Reset();
}
