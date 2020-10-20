#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"


void Display::Init()
{
    /*
    PainterMem_SetBuffer(buffer, 100, 100);
    PainterMem_FillRect(0, 0, 99, 99, ColorChannel(Channel::A));
    PainterMem_DrawRectangle(10, 10, 10, 30, COLOR_FILL);
    PainterMem_DrawHLine(90, 10, 90, ColorChannel(Channel::B));
    PainterMem_DrawVLine(90, 10, 90, COLOR_BACK);
    PainterMem_DrawRectangle(0, 0, 99, 99, COLOR_FILL);
    */

    Painter::ResetFlash();

    Font::Set(TypeFont::_8);

    Painter::LoadPalette();
}
