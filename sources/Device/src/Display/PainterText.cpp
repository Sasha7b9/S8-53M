#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Display/Font/Font_c.h"
#include "common/Utils/Math_c.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


using namespace Primitives;


bool ByteFontNotEmpty(int eChar, int byte)
{
    static pUCHAR bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = Font::font->symbol[prevChar].bytes;
    }
    return bytes[byte] != 0;
}


int Painter::DrawFormatText(int x, int y, Color::E color, char *text, ...)
{
#undef SIZE_BUFFER
#define SIZE_BUFFER 200
    char buffer[SIZE_BUFFER];
    std::va_list args;
    va_start(args, text);
    std::vsprintf(buffer, text, args);
    va_end(args);
    return Text(buffer).Draw(x, y, color);
#undef SIZE_BUFFER
}


void Painter::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2)
{
    Char(symbol1).Draw(x, y, color1);
    Char(symbol2).Draw(x, y, color2);
}
