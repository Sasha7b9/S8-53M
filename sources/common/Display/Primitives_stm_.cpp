#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "Display/Display.h"
#include "common/Utils/Math_.h"


void Primitives::Point::Draw(int, int)
{

}


void Primitives::VLine::Draw(int, int, int, Color)
{

}


void Primitives::MultiVPointLine::Draw(int y, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta); //-V2563
    }
}


void Primitives::Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    HLine().Draw(y, x, x + width);
    VLine().Draw(x, y, y + height);
    HLine().Draw(y + height, x, x + width);
    if (x + width < Display::WIDTH)
    {
        HLine().Draw(x + width, y, y + height);
    }
}


void Primitives::MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta); //-V2563
    }
}


void Primitives::Region::Fill(int x, int y, Color color)
{
    for (int i = y; i <= y + height; i++)
    {
        HLine().Draw(i, x, x + width, color);
    }
}
