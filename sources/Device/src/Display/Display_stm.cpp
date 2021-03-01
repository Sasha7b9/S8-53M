#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"


static const int BUFFER_WIDTH = Display::WIDTH / 2;
static const int SIZE_BUFFER = BUFFER_WIDTH * Display::HEIGHT;

static uint8 front[SIZE_BUFFER] __attribute__((section("CCM_DATA")));
static uint8 back[SIZE_BUFFER];


void Display::Init()
{
    Color::ResetFlash();

    Font::Set(TypeFont::S8);

//    HAL_DAC1::Init();

    HAL_LTDC::Init(front, back);
}


uint8 *Display::GetBuffer()
{
    return back;
}


uint8 *Display::GetBufferEnd()
{
    return back + SIZE_BUFFER;
}


void Primitives::HLine::Draw(int y, int x0, int x1, Color color)
{
    color.SetAsCurrent();

    if (x0 < 0) { x0 = 0; }
    if (x1 < 0) { x1 = 0; }
    if (y < 0) { y = 0; }

    if (x0 > x1)
    {
        Math::Swap(&x0, &x1);
    }

    uint8 *address = Display::GetBuffer() + x0 + y * BUFFER_WIDTH; //-V2563
    uint8 *end = Display::GetBufferEnd();

    uint8 value = Color::GetCurrent().index;

    for (int x = x0; x <= x1; ++x)
    {
        if (address >= end)
        {
            break;
        }
        *address++ = value;
    }
}
