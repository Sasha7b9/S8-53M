#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "Display/Display.h"
#include "common/Utils/Math_.h"


extern uint8 *display_back_buffer;


void Primitives::Point::Draw(int x, int y)
{
    BoundingX(x);
    BoundingY(y);

    *(display_back_buffer + Display::WIDTH * y + x) = Color::GetCurrent().index;
}


void Primitives::VLine::Draw(int x, int y0, int y1, const Color &color)
{
    color.SetAsCurrent();

    BoundingX(x);
    BoundingY(y0);
    BoundingY(y1);

    if (y1 < y0)
    {
        Math::Swap(&y0, &y1);
    }

    uint8 *address = display_back_buffer + Display::WIDTH * y0 + x;

    int counter = y1 - y0 + 1;

    uint8 value = Color::GetCurrent().index;

    do 
    {
        *address = value;
        address += Display::WIDTH;

    } while (--counter > 0);
}


void Primitives::MultiVPointLine::Draw(int y, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta); //-V2563
    }
}


void Primitives::Rectangle::Draw(int x, int y, const Color &color)
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


void Primitives::Region::Fill(int x, int y, const Color &color)
{
    for (int i = y; i <= y + height; i++)
    {
        HLine().Draw(i, x, x + width, color);
    }
}
