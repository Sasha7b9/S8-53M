#include "defines.h"
#include "common/Log_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "common/Hardware/HAL/HAL_PINS_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Painter.h"
#include "LAN/SocketTCP.h"
#include "VCP/VCP.h"


using namespace Primitives;


static bool inverseColors = false;
static Color::E currentColor = Color::NUM;

StateTransmit::E Painter::stateTransmit = StateTransmit::Free;

bool Painter::noFonts = false;


void Painter::SendFrame(bool first, bool noFonts_)
{
    noFonts = noFonts_;

    if (stateTransmit == StateTransmit::Free)
    {
        stateTransmit = (first ? StateTransmit::NeedForTransmitFirst : StateTransmit::NeedForTransmitSecond);
    }
}


void Painter::CalculateCurrentColor()
{
    if (currentColor == Color::FLASH_10)
    {
        Color::SetCurrent(inverseColors ? Color::BACK : Color::FILL);
    }
    else if (currentColor == Color::FLASH_01)
    {
        Color::SetCurrent(inverseColors ? Color::FILL : Color::BACK);
    }
}


void CalculateColor(uint8 *color)
{
    currentColor = (Color::E)*color;
    if (*color == Color::FLASH_10)
    {
        *color = static_cast<uint8>(inverseColors ? Color::BACK : Color::FILL);
    }
    else if (*color == Color::FLASH_01)
    {
        *color = static_cast<uint8>(inverseColors ? Color::FILL : Color::BACK);
    }
}


void InverseColor(Color::E *color)
{
    *color = (*color == Color::BLACK) ? Color::WHITE : Color::BLACK;
}


static void OnTimerFlashDisplay()
{
    inverseColors = !inverseColors;
}


void Painter::ResetFlash()
{
    Timer::Enable(TypeTimer::FlashDisplay, 400, OnTimerFlashDisplay);
    inverseColors = false;
}


void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            HLine().Draw(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        HLine().Draw(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}


void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            HLine().Draw(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        HLine().Draw(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}


static int numberColorsUsed = 0;



void Painter::DrawLineC(int x0, int y0, int x1, int y1, Color::E color)
{
    Color::SetCurrent(color);
    DrawLine(x0, y0, x1, y1);
}


void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += static_cast<int>(delta))
    {
        Point().Draw(x, y);
    }
}


void Painter::DrawLine(int x0, int y0, int x1, int y1)
{
    if (x0 == x1)
    {
        HLine().Draw(x0, y0, y1);
    }
    else if (y0 == y1)
    {
        HLine().Draw(y0, x0, x1);
    }
}


void Painter::DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade)
{
    if (inShade)
    {
        thickness = 1;
    }

    Region(width - thickness * 2, height - thickness * 2).Fill(x + thickness, y + thickness, normal);

    if (isPressed || inShade)
    {
        for (int i = 0; i < thickness; i++)
        {
            HLine().Draw(y + i, x + i, x + width - i, dark);
            VLine().Draw(x + i, y + 1 + i, y + height - i);
            VLine().Draw(x + width - i, y + 1 + i, y + height - i, bright);
            HLine().Draw(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            HLine().Draw(y + i, x + i, x + width - i, bright);
            VLine().Draw(x + i, y + 1 + i, y + height - i);
            VLine().Draw(x + width - i, y + 1 + i, y + height - i, dark);
            HLine().Draw(y + height - i, x + 1 + i, x + width - i);
        }
    }
}


void Painter::SetBrightnessDisplay(int16 )
{
}


int NumberColorsInSceneCol(void)
{
    return numberColorsUsed;
}


void Painter::DrawVLineArray(int , int , uint8 *, Color::E )
{
}


void Painter::DrawSignal(int , uint8 [281], bool )
{
}


void Painter::RunDisplay()
{
}


Color::E Painter::GetColor(int , int )
{
    return Color::WHITE;
}


void Painter::Get8Points(int , int , uint8 [4])
{
}


uint8 Get2Points(int , int )
{
    return 0;
}


void Painter::DrawPicture(int , int , int , int , uint8 *)
{
}


uint16 Painter::ReduceBrightness(uint16 colorValue, float newBrightness)
{
    int red = static_cast<int>(R_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(red, red, 0, 31);
    int green = static_cast<int>(G_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(green, green, 0, 63);
    int blue = static_cast<int>(B_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(blue, blue, 0, 31);
    return MAKE_COLOR(red, green, blue);
}


bool Painter::SaveScreenToFlashDrive(void)
{
    return true;
}
