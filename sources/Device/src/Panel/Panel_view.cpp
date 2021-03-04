// 2021/03/04 8:23:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "common/Display/Primitives_.h"
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
    if (data[1] < Key::Count)
    {
        keys[data[1]] = true;
    }
}


void Panel::Draw()
{
    static uint prev = HAL_TIM2::TimeMS();

    if (HAL_TIM2::TimeMS() < prev + 50)
    {
        return;
    }

    prev = HAL_TIM2::TimeMS();

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
