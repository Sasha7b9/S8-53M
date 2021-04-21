// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"


extern uint8 *display_back_buffer;
extern uint8 *display_back_buffer_end;


void Point::Draw(int x, int y) const
{
    if (x < 0 || y < 0)
    {
        return;
    }

    uint8 *address = display_back_buffer + Display::WIDTH * y + x;

    if (address < display_back_buffer_end)
    {
        *address = Color::GetCurrent().index;
    }
}


void VLine::Draw(int x, int y0, int y1) const
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


void VLine::Draw(int x, int y) const
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


void Region::Fill(int x, int y) const
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
