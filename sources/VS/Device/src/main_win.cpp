#include "defines.h"
#include "device.h"
#include "Settings/Settings.h"



void init()
{
    Device::Init();
    ShowMenu(true);
}



void update()
{
    Device::Update();
}
