// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/HAL/HAL_PINS_.h"
#include "common/Utils/Math_.h"


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
    else if (currentColor == Color::FLASH_01)
    {
        inverseColors ? Color::FILL.SetAsCurrent() : Color::BACK.SetAsCurrent();
    }
}


void CalculateColor(uint8 *color)
{
    currentColor.index = *color;

    if (*color == Color::FLASH_10.index)
    {
        *color = (uint8)(inverseColors ? Color::BACK.index : Color::FILL.index);
    }
    else if (*color == Color::FLASH_01.index)
    {
        *color = (uint8)(inverseColors ? Color::FILL.index : Color::BACK.index);
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
    int red = Math::Limitation((int)(R_FROM_COLOR(colorValue) * newBrightness), 0, 31);

    int green = Math::Limitation((int)(G_FROM_COLOR(colorValue) * newBrightness), 0, 63);

    int blue = Math::Limitation((int)(B_FROM_COLOR(colorValue) * newBrightness), 0, 31);

    return Color::Make((uint8)(red), (uint8)(green), (uint8)(blue));
}


bool Painter::SaveScreenToFlashDrive()
{
    return true;
}
