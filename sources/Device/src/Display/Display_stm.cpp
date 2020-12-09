#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Display/Display.h"


static uint8 front[Display::WIDTH * Display::HEIGHT];
static uint8 back[Display::WIDTH * Display::HEIGHT];


void Display::Init()
{
    Color::ResetFlash();

    Font::Set(TypeFont::S8);

    HAL_LTDC::Init((uint)front, (uint)back);
}
