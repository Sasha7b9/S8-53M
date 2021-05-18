// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"
#include "Settings/SettingsCommon.h"


int main()
{
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();
    
    Device::Init();
       
    while(1)
    {
        Device::Update();
//        Panel::Draw();
    }
}
