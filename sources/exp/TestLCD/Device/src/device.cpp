    // (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Hardware/HAL/HAL_.h"
#include "Display/Display.h"


bool Device::needResetSettings = false;


void Device::Init()
{
    HAL::Init();
    Display::Init();
}


void Device::Update()
{
    Display::Update();
}
