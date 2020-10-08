#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"


void Primitives::Point::Draw(int, int)
{

}


void Primitives::VLine::Draw(int, int, int, Color::E)
{

}


void Primitives::MultiVPointLine::Draw(int y, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Painter::SetColor(color);

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
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


void Primitives::Region::Fill(int, int, Color::E)
{

}


void Primitives::HLine::Draw(int, int, int, Color::E)
{

}
