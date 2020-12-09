#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Display/Display.h"


void Display::Init()
{
    Color::ResetFlash();

    Font::Set(TypeFont::S8);

    HAL_LTDC::Init()
}
