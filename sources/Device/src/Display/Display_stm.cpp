#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include <cstdlib>
#include <cstring>


static const int SIZE_BUFFER = Display::WIDTH * Display::HEIGHT;

static uint8 *front = (uint8 *)HAL_FMC::ADDR_RAM_DISPLAY_FRONT;
static uint8 back[240][320];

uint8 *display_back_buffer = &back[0][0];


void Display::Init()
{
    Color::ResetFlash();

    Font::Set(TypeFont::S8);

    HAL_DAC1::Init();

    HAL_LTDC::Init(front, &back[0][0]);
}


void Display::BeginFrame(const Color &color)
{
    std::memset(back, color.index, SIZE_BUFFER);
}


void Display::EndFrame()
{
    HAL_LTDC::ToggleBuffers();
}


uint8 *Display::GetBuffer()
{
    return &back[0][0];
}


uint8 *Display::GetBufferEnd()
{
    return GetBuffer() + SIZE_BUFFER;
}


void Primitives::HLine::Draw(int y, int x1, int x2, const Color &color) const
{
    color.SetAsCurrent();

    Draw(y, x1, x2);
}


void Primitives::HLine::Draw(int y, int x0, int x1) const
{
    if (y < 0 || y >= Display::HEIGHT)
    {
        return;
    }

    BoundingX(x0);
    BoundingX(x1);

    if (x0 > x1)
    {
        Math::Swap(&x0, &x1);
    }

    uint8 *start = &back[y][x0];

    std::memset(start, Color::GetCurrent().index, (uint)(x1 - x0 + 1));
}
