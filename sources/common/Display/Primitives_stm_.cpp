#include "defines.h"
#include "common/Utils/String_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "Display/Display.h"
#include "common/Utils/Math_.h"


extern uint8 *display_back_buffer;


void Primitives::Point::Draw(int x, int y) const
{
    BoundingX(x);
    BoundingY(y);

    *(display_back_buffer + Display::WIDTH * y + x) = Color::GetCurrent().index;
}


void Primitives::VLine::Draw(int x, int y0, int y1) const
{
    if (x < 0 || x >= Display::WIDTH)
    {
        return;
    }

    BoundingY(y0);
    BoundingY(y1);

    Math::Sort(&y0, &y1);

    uint8 *address = display_back_buffer + Display::WIDTH * y0 + x;

    int counter = y1 - y0 + 1;

    uint8 value = Color::GetCurrent().index;

    do 
    {
        *address = value;
        address += Display::WIDTH;

    } while (--counter > 0);
}


void Primitives::VLine::Draw(int x, int y) const
{
    if (x < 0 || x >= Display::WIDTH)
    {
        return;
    }

    BoundingY(y);

    uint8 *address = display_back_buffer + Display::WIDTH * y + x;

    uint8 value = Color::GetCurrent().index;

    for (int i = 0; i < height; i++)
    {
        *address = value;
        address += Display::WIDTH;
    }
}


void Primitives::Rectangle::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Primitives::Rectangle::Draw(int x, int y) const
{
    if (width == 0 || height == 0)
    {
        return;
    }

    int dx = x + width - 1;
    int dy = y + height - 1;

    HLine().Draw(y, x, dx);
    HLine().Draw(dy, x, dx);
    VLine().Draw(x, y, dy);
    VLine().Draw(dx, y, dy);
}


void Primitives::MultiHPointLine::Draw(int x, const Color &color) const
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


void Primitives::Region::Fill(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Fill(x, y);
}


void Primitives::Region::Fill(int x, int y) const
{
    if (width == 0 || height == 0)
    {
        return;
    }

    for (int i = y; i <= y + height - 1; i++)
    {
        HLine().Draw(i, x, x + width - 1);
    }
}
