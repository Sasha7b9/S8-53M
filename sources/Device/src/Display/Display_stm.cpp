// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include <cstdlib>
#include <cstring>


static const int SIZE_BUFFER = Display::WIDTH * Display::HEIGHT;

static uint8 *front = (uint8 *)HAL_FMC::ADDR_RAM_DISPLAY_FRONT;
static uint8 back[240][320];

uint8 *display_back_buffer = &back[0][0];
uint8 *display_back_buffer_end = display_back_buffer + SIZE_BUFFER;


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


void HLine::Draw(int y, int x0, int x1) const
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

void HLine::Draw(int x, int y) const
{
    if (y < 0 || y >= Display::HEIGHT)
    {
        return;
    }

    int w = width;  

    if (x < 0)
    {
        w += x;
        x = 0;
    }
    else if (x >= Display::WIDTH)
    {
        w -= (x - Display::WIDTH);
        x = Display::WIDTH - 1;
    }

    uint8 *address = &back[y][x];

    std::memset(address, Color::GetCurrent().index, (uint)w);
}

