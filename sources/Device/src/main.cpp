// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"


int main()
{
    /*
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();
   
    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase(); 
    */

    setNRST.Load();


    Device::Init();

    while(1)
    {
        Device::Update();

        set.Save();

        setNRST.Save();
    }
}
