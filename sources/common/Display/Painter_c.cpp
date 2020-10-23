#include "defines.h"
#include "common/Log_c.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "common/Hardware/HAL/HAL_PINS_c.h"
#include "common/Utils/Math_c.h"


using namespace Primitives;


static bool inverseColors = false;
static Color::E currentColor = Color::Count;
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


Color::E Painter::GetColor(int , int )
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


bool Painter::SaveScreenToFlashDrive()
{
    return true;
}
