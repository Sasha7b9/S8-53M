#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "GUI/Application.h"


#pragma warning(push, 0)
#undef A
#include <wx/wx.h>
#pragma warning(pop)


void Point::Draw(int x, int y) const
{
    Application::memDC.DrawPoint({ x, y });
}


void HLine::Draw(int y, int x1, int x2, const Color &color) const
{
    color.SetAsCurrent();

    Draw(y, x1, x2);
}


void HLine::Draw(int y, int x1, int x2) const
{
    Math::Sort(&x1, &x2);

    for (int x = x1; x <= x2; x++)
    {
        Point().Draw(x, y);
    }
}


void VLine::Draw(int x, int y0, int y1) const
{
    Math::Sort(&y0, &y1);

    for (int y = y0; y <= y1; y++)
    {
        Point().Draw(x, y);
    }
}


void VLine::Draw(int x, int y) const
{
    for (int i = 0; i < height; i++)
    {
        Point().Draw(x, y++);
    }
}
