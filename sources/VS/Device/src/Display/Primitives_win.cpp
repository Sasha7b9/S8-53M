#include "defines.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include "common/Display/Font/Font.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


using namespace Primitives;


extern wxMemoryDC memDC;


void Primitives::Point::Draw(int x, int y)
{
    memDC.DrawPoint({ x, y });
}


void Primitives::Region::Fill(int x, int y, Color::E color)
{
    Color::SetCurrent(color);
    wxBrush brush = memDC.GetBrush();
    wxPen pen = memDC.GetPen();
    memDC.SetBrush(wxBrush(pen.GetColour()));
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    memDC.SetBrush(brush);
}


void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Primitives::HLine::Draw(int y, int x1, int x2, Color::E color)
{
    Color::SetCurrent(color);

    memDC.DrawLine({ x1, y }, { x2, y });
}


void Primitives::VLine::Draw(int x, int y0, int y1, Color::E color)
{
    Color::SetCurrent(color);
    memDC.DrawLine({ x, y0 }, { x, y1 });
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

