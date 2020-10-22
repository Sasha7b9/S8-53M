#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"


void Display::Init()
{
    Color::ResetFlash();

    Font::Set(TypeFont::_8);
}
