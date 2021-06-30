// 2021/06/30 15:45:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "main.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Display/Display.h"


static bool running = false;


void Display::Update()
{
    running = true;

    BeginFrame(Color::BLACK);

    if (MainStruct::state == State::Upgrade)
    {
        Text("��������� ����������").DrawInCenterRect(0, 0, 320, 190);
        Text("��������� ������������ �����������").DrawInCenterRect(0, 0, 320, 220);

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
