// 2021/06/30 15:45:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"


static bool running = false;


void Display::Update()
{
    running = true;

    BeginFrame(Color::BLACK);

    EndFrame();

    running = false;
}


bool Display::IsRunning()
{
    return running;
}
