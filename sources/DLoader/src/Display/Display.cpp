// 2021/06/30 15:45:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"


void Display::Update()
{
    BeginFrame(Color::BLACK);

    EndFrame();
}


bool Display::IsRun()
{
    return false;
}
