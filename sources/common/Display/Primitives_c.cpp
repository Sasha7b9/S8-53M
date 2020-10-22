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
