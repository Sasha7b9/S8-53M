#include "defines.h"
#include "common/Log_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"


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


void Primitives::VPointLine::Draw(int , int , int , const Color &) const
{

}


void Primitives::HPointLine::Draw(int y, int x0, int x1) const
{
    for (int x = x0; x <= x1; x += (int)(delta))
    {
        Point().Draw(x, y);
    }
}


void Primitives::Line::Draw(int x0, int y0, int x1, int y1, const Color &color) const
{
    color.SetAsCurrent();

    if (x0 == x1)
    {
        HLine().Draw(x0, y0, y1);
    }
    else if (y0 == y1)
    {
        HLine().Draw(y0, x0, x1);
    }
}


void Primitives::DashedHLine::Draw(int y, int x0, int x1, int deltaStart) const
{
    if (deltaStart < 0 || deltaStart >= (fill + empty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (fill + empty - deltaStart);
        if (deltaStart < fill)     // Если начало линии приходится на штрих
        {
            HLine().Draw(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        HLine().Draw(y, x, x + fill - 1);
        x += (fill + empty);
    }
}


void Primitives::DashedVLine::Draw(int x, int y0, int y1, int deltaStart) const
{
    if (deltaStart < 0 || deltaStart >= (fill + empty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (fill + empty - deltaStart);
        if (deltaStart < fill)     // Если начало линии приходится на штрих
        {
            HLine().Draw(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        HLine().Draw(x, y, y + fill - 1);
        y += (fill + empty);
    }
}


void Primitives::VLineArray::Draw(int , int , uint8 *, const Color &color) const
{
    color.SetAsCurrent();
}


void Primitives::VLine::Draw(int x, int y0, int y1, const Color &color) const
{
    color.SetAsCurrent();

    Draw(x, y0, y1);
}


void Primitives::VLine::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Primitives::MultiVPointLine::Draw(int y, const Color &color) const
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


void Primitives::Rectangle::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Primitives::Rectangle::Draw(int x, int y) const
{
    if (width == 0 || height == 0)
    {
        return;
    }

    int dx = x + width - 1;
    int dy = y + height - 1;

    HLine().Draw(y, x, dx);
    HLine().Draw(dy, x, dx);
    VLine().Draw(x, y, dy);
    VLine().Draw(dx, y, dy);
}


void Primitives::MultiHPointLine::Draw(int x, const Color &color) const
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


void Primitives::Region::Fill(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    Fill(x, y);
}


void Primitives::Region::Fill(int x, int y) const
{
    if (width == 0 || height == 0)
    {
        return;
    }

    for (int i = y; i <= y + height - 1; i++)
    {
        HLine().Draw(i, x, x + width - 1);
    }
}
