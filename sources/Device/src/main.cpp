#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"

int main(void)
{    
    Device::Init();
    
    while(1)
    {
        Device::Update();
    }
}
