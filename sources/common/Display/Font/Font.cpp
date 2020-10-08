

#include "defines.h"
#include "Display/DisplayTypes.h"
#include "font.h"

#include "font8display.inc"
#include "font5display.inc"
#include "fontUGOdisplay.inc"
#include "fontUGO2display.inc"

#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"



const Font *fonts[TypeFont::_Count] = {&font5, &font8, &fontUGO, &fontUGO2};
const Font *font = &font8;



int Font::GetSize()
{
    return font->height;
}



int Font::GetLengthText(const char *text)
{
    int retValue = 0;
    while (*text)
    {
        retValue += Font_GetLengthSymbol(static_cast<uint8>(*text));
        text++;
    }
    return retValue;
}



int Font_GetHeightSymbol(char)
{
    return 9;
}



int Font_GetLengthSymbol(uchar symbol)
{
    return font->symbol[symbol].width + 1;
}
