// 2021/03/04 8:23:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "common/Display/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Panel/Panel.h"


using namespace Primitives;


static bool keys[Key::Count] =
{
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, false
};


void Panel::CallbackOnReceiveSPI5(uint8 *data, int /*size*/)
{
    uint8 key = data[1];

//    uint8 action = data[2];
//
//    if (action == 0)
//    {
//        Sound::ButtonPress();
//    }
//    else if (action == 1 || action == 2)
//    {
//        Sound::ButtonRelease();
//    }
//    else if (action == 3 || action == 4)
//    {
//        Sound::RegulatorShiftRotate();
//    }

    if (key < Key::Count)
    {
        keys[key] = true;
    }
}


void Panel::Draw()
{
    Display::BeginScene(Color::WHITE);

    int x0 = 10;
    int y0 = 10;

    int height = 3;

    for (int i = 0; i < Key::Count; i++)
    {
        if (keys[i])
        {
            Region(i * 5, height).Fill(x0, y0 + (height + 2) * i, Color::BLACK);
        }
        else
        {
            Rectangle(i * 5, height).Draw(x0, y0 + (height + 2) * i, Color::BLACK);
        }
    }
}
