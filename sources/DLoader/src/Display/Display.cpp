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

    Border::Draw();

    if (MainStruct::state == State::UpdateInProgress)
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


void Display::Border::Draw()
{
    for (int i = 0; i < 10; i++)
    {
        uint value = TIME_MS / 5;

        BitSet64 coord = CalculateCoord(value + i * 10);
        
        Point().Draw(coord.first, coord.second);
    }
}


BitSet64 Display::Border::CalculateCoord(uint value)
{
    int perimeter = Display::WIDTH * 2 + Display::HEIGHT * 2;

    value %= perimeter;

    if (value < Display::WIDTH)
    {
        return BitSet64((int)value, 0);
    }
    else if (value < Display::WIDTH + Display::HEIGHT)
    {
        return BitSet64(Display::WIDTH - 1, (int)value - Display::WIDTH);
    }
    else if (value < Display::WIDTH * 2 + Display::HEIGHT)
    {
        return BitSet64(Display::WIDTH - ((int)value - Display::WIDTH - Display::HEIGHT), Display::HEIGHT - 1);
    }

    return BitSet64(0, Display::HEIGHT - ((int)value - Display::WIDTH * 2 - Display::HEIGHT));
}
