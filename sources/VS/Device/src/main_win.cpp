#include "defines.h"
#include "device.h"
#include "GUI/Application.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"



void Application::Init()
{
    Device::Init();
    Menu::Show(true);
}



void Application::Update()
{
    Device::Update();
}
