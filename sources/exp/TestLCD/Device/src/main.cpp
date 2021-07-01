// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


int main()
{
    /*
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();
   
    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase(); 
    */
    
    Device::Init();

    while(1)
    {
        Device::Update();
    }
}
