#include "defines.h"
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
