#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Font/Font_c.h"
#include "GUI/Application.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


using namespace Primitives;


void Primitives::Point::Draw(int x, int y)
{
    Application::memDC.DrawPoint({ x, y });
}


void Primitives::Region::Fill(int x, int y, Color::E color)
{
    Color::SetCurrent(color);
    wxBrush brush = Application::memDC.GetBrush();
    wxPen pen = Application::memDC.GetPen();
    Application::memDC.SetBrush(wxBrush(pen.GetColour()));
    Application::memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    Application::memDC.SetBrush(brush);
}


void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    Application::memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Primitives::HLine::Draw(int y, int x1, int x2, Color::E color)
{
    Color::SetCurrent(color);

    Application::memDC.DrawLine({ x1, y }, { x2, y });
}


void Primitives::VLine::Draw(int x, int y0, int y1, Color::E color)
{
    Color::SetCurrent(color);
    Application::memDC.DrawLine({ x, y0 }, { x, y1 });
}


void Primitives::MultiVPointLine::Draw(int y, Color::E color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


void Primitives::MultiHPointLine::Draw(int x, Color::E color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}

