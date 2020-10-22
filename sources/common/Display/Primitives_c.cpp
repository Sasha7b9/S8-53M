#include "defines.h"
#include "Display/Painter.h"
#include "common/Display/Primitives_c.h"


void Primitives::MultiVPointLine::DrawVPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        Point().Draw(x, y);
        y += delta;
    }
}


void Primitives::MultiHPointLine::DrawHPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        Point().Draw(x, y);
        x += delta;
    }
}


void Primitives::VPointLine::Draw(int , int , int , Color::E )
{

}


void Primitives::HPointLine::Draw(int y, int x0, int x1)
{
    for (int x = x0; x <= x1; x += static_cast<int>(delta))
    {
        Point().Draw(x, y);
    }
}


void Primitives::Line::Draw(int x0, int y0, int x1, int y1)
{
    if (x0 == x1)
    {
        HLine().Draw(x0, y0, y1);
    }
    else if (y0 == y1)
    {
        HLine().Draw(y0, x0, x1);
    }
}
