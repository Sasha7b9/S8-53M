#include "defines.h"
#include "Display/Primitives.h"


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
