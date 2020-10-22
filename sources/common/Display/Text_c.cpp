#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "Display/Painter.h"
#include <cstring>


static bool ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;

    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = Font::font->symbol[prevChar].bytes;
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
        prevByte = Font::font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }

    return prevByte & (1 << bit);
}


static void DrawCharInColorDisplay(int eX, int eY, uchar symbol)
{
    int8 width = static_cast<int8>(Font::font->symbol[symbol].width);
    int8 height = static_cast<int8>(Font::font->height);

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
    int8 width = (int8)Font::font->symbol[symbol].width;
    int8 height = (int8)Font::font->height;

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


static int DrawBigChar(int eX, int eY, int size, char symbol)
{
    int8 width = static_cast<int8>(Font::font->symbol[symbol].width);
    int8 height = static_cast<int8>(Font::font->height);

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            Primitives::Point().Draw(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void Text::DrawBig(int eX, int eY, int size)
{
    int numSymbols = static_cast<int>(std::strlen(text.c_str()));

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}


void Char::Draw4SymbolsInRect(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < 2; i++)
    {
        Char(static_cast<char>(symbol + i)).Draw(x + 8 * i, y);
        Char(static_cast<char>(symbol + i + 16)).Draw(x + 8 * i, y + 8);
    }
}


int Text::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color::E color)
{
    Color::SetCurrent(color);

    int lenght = Font::GetLengthText(text.c_str());
    int height = Font::GetHeightSymbol();
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;
    return Draw(x, y);
}


void Text::DrawRelativelyRight(int xRight, int y, Color::E color)
{
    Color::SetCurrent(color);

    int lenght = Font::GetLengthText(text.c_str());
    Draw(xRight - lenght, y);
}
