#include "defines.h"
#include "common/Log_c.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "common/Hardware/HAL/HAL_PINS_c.h"
#include "common/Utils/Math_c.h"
#include "Settings/Settings.h"


using namespace Primitives;


static bool inverseColors = false;
static Color currentColor = Color::Count;
bool Painter::noFonts = false;
static int numberColorsUsed = 0;


void Painter::SendFrame(bool noFonts_)
{
    noFonts = noFonts_;
}


void Painter::CalculateCurrentColor()
{
    if (currentColor == Color::FLASH_10)
    {
        inverseColors ? Color::BACK.SetAsCurrent() : Color::FILL.SetAsCurrent();
    }
    else if (currentColor == Color::FLASH_01) //-V2516
    {
        inverseColors ? Color::FILL.SetAsCurrent() : Color::BACK.SetAsCurrent();
    }
}


void CalculateColor(uint8 *color)
{
    currentColor.index = *color; //-V2533

    if (*color == Color::FLASH_10.index)
    {
        *color = static_cast<uint8>(inverseColors ? Color::BACK.index : Color::FILL.index);
    }
    else if (*color == Color::FLASH_01.index) //-V2516
    {
        *color = static_cast<uint8>(inverseColors ? Color::FILL.index : Color::BACK.index);
    }
}


void InverseColor(Color *color)
{
    *color = (*color == Color::BLACK) ? Color::WHITE : Color::BLACK;
}


void Painter::SetBrightnessDisplay(int16 )
{
}


int NumberColorsInSceneCol()
{
    return numberColorsUsed;
}


void Painter::DrawSignal(int , uint8 [281], bool )
{
}


void Painter::RunDisplay()
{
}


Color Painter::GetColor(int , int )
{
    return Color::WHITE;
}


uint8 Get2Points(int , int )
{
    return 0;
}


void Painter::DrawPicture(int , int , int , int , uint8 *)
{
}


uint Painter::ReduceBrightness(uint colorValue, float newBrightness)
{
    int red = static_cast<int>(R_FROM_COLOR(colorValue) * newBrightness); //-V2564
    LIMITATION(red, red, 0, 31); //-V2516
    int green = static_cast<int>(G_FROM_COLOR(colorValue) * newBrightness); //-V2564
    LIMITATION(green, green, 0, 63); //-V2516
    int blue = static_cast<int>(B_FROM_COLOR(colorValue) * newBrightness); //-V2564
    LIMITATION(blue, blue, 0, 31); //-V2516
    return Color::Make(static_cast<uint8>(red), static_cast<uint8>(green), static_cast<uint8>(blue));
}


bool Painter::SaveScreenToFlashDrive()
{
    return true;
}
