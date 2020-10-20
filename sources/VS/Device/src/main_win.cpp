#include "defines.h"
#include "device.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"



void init()
{
    Device::Init();
    Menu::Show(true);
}



void update()
{
    Device::Update();
}
