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
