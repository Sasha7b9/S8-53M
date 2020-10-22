#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Display/Font/Font_c.h"
#include "common/Utils/Math_c.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstring>


using namespace Primitives;


void Painter::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2)
{
    Char(symbol1).Draw(x, y, color1);
    Char(symbol2).Draw(x, y, color2);
}
