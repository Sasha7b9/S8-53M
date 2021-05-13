#include "defines.h"
#include "common/Display/Painter/Text_.h"
#include "device.h"
#include "GUI/Application.h"
#include "Menu/Menu.h"
#include "Settings/SettingsCommon.h"



void Application::Init()
{
    Device::Init();
    Menu::Show(true);
}



void Application::Update()
{
    Device::Update();
}
