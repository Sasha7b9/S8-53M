#include "defines.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"


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


static bool ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
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

    char *pointer = text.c_str();

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


static void DrawCharInColorDisplay(int eX, int eY, uchar symbol)
{
    int8 width = static_cast<int8>(font->symbol[symbol].width);
    int8 height = static_cast<int8>(font->height);

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
                    Primitives::Point().Draw(x, y);
                }
                x++;
            }
        }
    }
}


int Primitives::Char::Draw(int x, int y, Color::E color)
{
    Painter::SetColor(color);

    Painter::CalculateCurrentColor();

    if (Font::GetSize() == 5)
    {
        Painter::DrawCharHardCol(x, y + 3, symbol);
    }
    else if (Font::GetSize() == 8)
    {
        Painter::DrawCharHardCol(x, y, symbol);
    }
    else
    {
        DrawCharInColorDisplay(x, y, static_cast<uint8>(symbol));
    }
    return x + Font::GetLengthSymbol(static_cast<uint8>(symbol));
}
