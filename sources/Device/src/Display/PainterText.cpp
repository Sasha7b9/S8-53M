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


void Painter::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, 
                                                 Color::E colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = Text(text).DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Fill(eX - lenght - widthBorder, y - widthBorder);
    Text(text).DrawInCenterRect(x, y, width, height, colorText);
}


int Painter::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill)
{
    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    Color::SetCurrent(colorFill);
    return Text(text).DrawInCenterRect(x, y, width, height);
}


void Painter::DrawHintsForSmallButton(int x, int y, int width, void *smallButton)
{
    SmallButton *sb = static_cast<SmallButton*>(smallButton);
    Region(width, 239 - y).Fill(x, y, Color::BACK);
    Rectangle(width, 239 - y).Draw(x, y, Color::FILL);
    const StructHelpSmallButton *structHelp = &(*sb->OwnData()->hintUGO)[0];
    x += 3;
    y += 3;
    while (structHelp->funcDrawUGO)
    {
        Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y);
        structHelp->funcDrawUGO(x, y);
        int yNew = Text(structHelp->helpUGO[LANG]).DrawInRectWithTransfers(x + 23, y + 1, width - 30, 20);
        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}


void Painter::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2)
{
    Char(symbol1).Draw(x, y, color1);
    Char(symbol2).Draw(x, y, color2);
}
