// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"
#include "Settings/SettingsCommon.h"


int main()
{
    Device::Init();
       
    while(1)
    {
        Device::Update();
    }
}
