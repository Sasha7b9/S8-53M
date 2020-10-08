#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Font/Font.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


extern wxMemoryDC memDC;


void Primitives::Point::Draw(int x, int y)
{
    memDC.DrawPoint({ x, y });
}


void Primitives::Region::Fill(int x, int y, Color::E color)
{
    Painter::SetColor(color);
    wxBrush brush = memDC.GetBrush();
    wxPen pen = memDC.GetPen();
    memDC.SetBrush(wxBrush(pen.GetColour()));
    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    memDC.SetBrush(brush);
}


void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Painter::SetColor(color);

    memDC.DrawRectangle({ x, y, width + 1, height + 1 });
}


void Primitives::HLine::Draw(int y, int x1, int x2, Color::E color)
{
    Painter::SetColor(color);
    memDC.DrawLine({ x1, y }, { x2, y });
}


void Primitives::VLine::Draw(int x, int y0, int y1, Color::E color)
{
    Painter::SetColor(color);
    memDC.DrawLine({ x, y0 }, { x, y1 });
}


void Primitives::MultiVPointLine::Draw(int y, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


void Primitives::MultiHPointLine::Draw(int x, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


static bool ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8* bytes = 0;
    static int prevChar = -1;

    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }

    return bytes[byte] != 0;
}


static bool BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;
    static int prevChar = -1;
    static int prevNumByte = -1;

    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }

    return prevByte & (1 << bit);
}


int Primitives::Text::Draw(int x, int y, Color::E color)
{
    Painter::SetColor(color);

    char* pointer = text.c_str();

    while (*pointer != '\0')
    {
        x = DrawChar(x, y, (uint8)(*pointer)) + 1;
        pointer++;
    }

    return x;
}


int Primitives::Text::DrawChar(int eX, int eY, uint8 symbol)
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    Point().Draw(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}
