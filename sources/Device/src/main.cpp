#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"


int main(void)
{    
    Device::Init();
    
    while(1)
    {
        // Device::Update();
        Panel::Draw();
    }
}
