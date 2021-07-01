// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_.h"
#include "Display/Display.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

static const int SIZE_BUFFER = Display::WIDTH * Display::HEIGHT;

static uint8 *front = (uint8 *)HAL_FMC::ADDR_RAM_DISPLAY_FRONT;
static uint8 back[240][320];

uint8 *display_back_buffer = &back[0][0];
uint8 *display_back_buffer_end = display_back_buffer + SIZE_BUFFER;


void Display::Init()
{
    Color::ResetFlash();

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



void Display::Update()
{
    BeginFrame(Color::BLACK);
    
    for(int x = 0; x < 320; x++)
    {
        for(int y = 0; y < 240; y++)
        {
            back[y][x] = (uint8)std::rand();
        }
    }

    EndFrame();
}
