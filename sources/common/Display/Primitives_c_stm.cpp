#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "Display/Display.h"


void Primitives::Point::Draw(int, int)
{

}


void Primitives::VLine::Draw(int, int, int, Color::E)
{

}


void Primitives::MultiVPointLine::Draw(int y, Color::E color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta); //-V2563
    }
}


void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    HLine().Draw(y, x, x + width);
    VLine().Draw(x, y, y + height);
    HLine().Draw(y + height, x, x + width);
    if (x + width < Display::WIDTH)
    {
        HLine().Draw(x + width, y, y + height);
    }
}


void Primitives::MultiHPointLine::Draw(int x, Color::E color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta); //-V2563
    }
}


void Primitives::Region::Fill(int, int, Color::E)
{

}


void Primitives::HLine::Draw(int, int, int, Color::E)
{

}
