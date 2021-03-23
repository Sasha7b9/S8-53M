#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Font/Font_.h"
#include "common/Utils/Math_.h"
#include "GUI/Application.h"


#pragma warning(push, 0)
#undef A
#include <wx/wx.h>
#pragma warning(pop)


using namespace Primitives;


void Primitives::Point::Draw(int x, int y) const
{
    Application::memDC.DrawPoint({ x, y });
}


void Primitives::Region::Fill(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Fill(x, y);
}


void Primitives::Region::Fill(int x, int y) const
{
    wxBrush brush = Application::memDC.GetBrush();
    wxPen pen = Application::memDC.GetPen();
    Application::memDC.SetBrush(wxBrush(pen.GetColour()));
    Application::memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    Application::memDC.SetBrush(brush);
}


void Primitives::Rectangle::Draw(int x, int y) const
{
    if (width == 1)
    {
        VLine().Draw(x, y, y + height);
    }
    else if (height == 1)
    {
        HLine().Draw(y, x, x + width);
    }
    else
    {
        Application::memDC.DrawRectangle({ x, y, width, height });
    }
}


void Primitives::HLine::Draw(int y, int x1, int x2, const Color &color) const
{
    color.SetAsCurrent();

    Draw(y, x1, x2);
}


void Primitives::HLine::Draw(int y, int x1, int x2) const
{
    Math::Sort(&x1, &x2);

    for (int x = x1; x <= x2; x++)
    {
        Point().Draw(x, y);
    }
}


void Primitives::VLine::Draw(int x, int y0, int y1) const
{
    Math::Sort(&y0, &y1);

    for (int y = y0; y <= y1; y++)
    {
        Point().Draw(x, y);
    }
}


void Primitives::VLine::Draw(int x, int y) const
{
    for (int i = 0; i < height; i++)
    {
        Point().Draw(x, y++);
    }
}


void Primitives::MultiHPointLine::Draw(int x, const Color &color) const
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}

