#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Font/Font_.h"
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


void Primitives::Rectangle::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Primitives::Rectangle::Draw(int x, int y) const
{
    Application::memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Primitives::HLine::Draw(int y, int x1, int x2, const Color &color) const
{
    color.SetAsCurrent();

    Draw(y, x1, x2);
}


void Primitives::HLine::Draw(int y, int x1, int x2) const
{
    Application::memDC.DrawLine({ x1, y }, { x2, y });
}


void Primitives::VLine::Draw(int x, int y0, int y1, const Color &color) const
{
    color.SetAsCurrent();

    Draw(x, y0, y1);
}


void Primitives::VLine::Draw(int x, int y0, int y1) const
{
    Application::memDC.DrawLine({ x, y0 }, { x, y1 });
}


void Primitives::MultiVPointLine::Draw(int y, const Color &color) const
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta); //-V2563
    }
}


void Primitives::MultiHPointLine::Draw(int x, const Color &color) const
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta); //-V2563
    }
}

