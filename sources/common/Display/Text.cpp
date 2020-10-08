#include "defines.h"
#include "common/Display/Primitives.h"
#include "common/Display/Text.h"
#include "Display/Painter.h"


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


static void DrawCharHardCol(int x, int y, char symbol)
{
    char str[2] = { symbol, 0 };
    Text(str).Draw(x, y);
}


int Char::Draw(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    Painter::CalculateCurrentColor();

    if (Font::GetSize() == 5)
    {
        DrawCharHardCol(x, y + 3, symbol);
    }
    else if (Font::GetSize() == 8)
    {
        DrawCharHardCol(x, y, symbol);
    }
    else
    {
        DrawCharInColorDisplay(x, y, static_cast<uint8>(symbol));
    }
    return x + Font::GetLengthSymbol(static_cast<uint8>(symbol));
}


int Text::Draw(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    char *pointer = text.c_str();

    while (*pointer != '\0')
    {
        x = DrawChar(x, y, (uint8)(*pointer)) + 1;
        pointer++;
    }

    return x;
}


int Text::DrawChar(int eX, int eY, uint8 symbol)
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
                    Primitives::Point().Draw(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}


static int GetLenghtSubString(char *text)
{
    int retValue = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        retValue += Font::GetLengthSymbol(static_cast<uint8>(*text));
        text++;
    }
    return retValue;
}


static int DrawSubString(int x, int y, char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Char(*text).Draw(x, y);
        numSymbols++;
        text++;
    }
    return numSymbols;
}


static int DrawSpaces(int x, int y, char *text, int *numSymbols)
{
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = Char(*text).Draw(x, y);
        text++;
        (*numSymbols)++;
    }
    return x;
}


void Text::DrawInRect(int x, int y, int width, int)
{
    int xStart = x;
    int xEnd = xStart + width;

    char *t = text.c_str();

    while (*t != 0)
    {
        int length = GetLenghtSubString(t);
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol();
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, t);
        t += numSymbols;
        x += length;
        x = DrawSpaces(x, y, t, &numSymbols);
        t += numSymbols;
    }
}
