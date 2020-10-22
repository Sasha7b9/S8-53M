#include "defines.h"
#include "common/Display/Painter_c.h"
#include "Display/Display.h"


void Display::Init()
{
    Color::ResetFlash();

    Font::Set(TypeFont::_8);
}
