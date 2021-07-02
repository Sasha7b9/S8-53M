// 2021/06/30 15:45:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "main.h"
#include "common/Display/Painter/Animated_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Display/Display.h"


static bool running = false;

static ABorder border(Display::WIDTH - 4, Display::HEIGHT - 4);


void Display::Update()
{
    if (running)
    {
        return;
    }

    running = true;

    BeginFrame(Color::BLACK);

    Rectangle(Display::WIDTH, Display::HEIGHT).Draw(0, 0, Color::WHITE);
    border.Draw(2, 2);
    Rectangle(Display::WIDTH - 8, Display::HEIGHT - 8).Draw(4, 4);

    if (MainStruct::state == State::DriveDetected)
    {
        DrawMessage("Обнаружен диск. Попытка подключения...");
    }
    else if (MainStruct::state == State::EraseSectors)
    {
        DrawMessage("Стираю сектора...");

        DrawProgressBar();
    }
    else if (MainStruct::state == State::UpdateInProgress)
    {
        DrawMessage("Подождите завершения", "установки программного обеспечения");

        DrawProgressBar();
    }

    EndFrame();

    running = false;
}


void Display::DrawProgressBar()
{
    int height = 30;
    int fullWidth = 280;
    int width = (int)((float)(fullWidth)*MainStruct::percentUpdate);

    Region(width, height).Fill(20, 130);
    Rectangle(fullWidth, height).Draw(20, 130);
}


bool Display::IsRunning()
{
    return running;
}


void Display::DrawMessage(pchar message1, pchar message2)
{
    Text(message1).DrawInCenterRect(0, 0, Display::WIDTH, 100);

    if (message2)
    {
        Text(message2).DrawInCenterRect(0, 0, Display::WIDTH, 120);
    }
}
