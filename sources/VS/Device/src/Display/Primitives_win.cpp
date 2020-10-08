#include "defines.h"
#include "Display/Primitives.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


extern wxMemoryDC memDC;


void Primitives::Point::Draw(int x, int y)
{
    memDC.DrawPoint({ x, y });
}
