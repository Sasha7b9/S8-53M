// 2021/06/30 15:45:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "main.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Display.h"


static bool running = false;


void Display::Update()
{
    running = true;

    BeginFrame(Color::BLACK);

    static int i = 0;
    i++;

    if (i > 10)
    {
        i = 0;
    }

    Region(10, 10).Fill(10 + (int)(i % 10) * 20, 10, Color::WHITE);

    if (MainStruct::state == State::Upgrade)
    {
        Text("Подождите завершения").DrawInCenterRect(0, 0, 320, 190);
        Text("установки программного обеспечения").DrawInCenterRect(0, 0, 320, 220);

        int height = 30;
        int fullWidth = 280;
        int width = (int)((float)(fullWidth)*MainStruct::percentUpdate);

        Region(width, height).Fill(20, 130);
        Rectangle(fullWidth, height).Draw(20, 130);
    }

    EndFrame();

    running = false;
}


bool Display::IsRunning()
{
    return running;
}
