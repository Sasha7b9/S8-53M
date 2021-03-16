#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"


int main()
{
    set.Load(true);
    
    Device::Init();
       
    while(1)
    {
        Device::Update();
//        Panel::Draw();
    }
}
