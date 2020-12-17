#include "defines.h"
#include "common/Log_c.h"
#include "common/Display/Colors_c.h"
#include "common/Utils/Math_c.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


static const uint8 NUM_COLORS = 16;


Color Color::BLACK(0);
Color Color::WHITE(1);
Color Color::GRID(2);
Color Color::DATA_A(3);
Color Color::DATA_B(4);
Color Color::MENU_FIELD(5);
Color Color::MENU_TITLE(6);
Color Color::MENU_TITLE_DARK(7);
Color Color::MENU_TITLE_BRIGHT(8);
Color Color::MENU_ITEM(9);
Color Color::MENU_ITEM_DARK(10);
Color Color::MENU_ITEM_BRIGHT(11);
Color Color::MENU_SHADOW(12);
Color Color::EMPTY(13);
Color Color::EMPTY_A(14);
Color Color::EMPTY_B(15);
Color Color::Count(NUM_COLORS);

Color Color::FLASH_10(NUM_COLORS + 1);
Color Color::FLASH_01(NUM_COLORS + 2);

Color Color::current = Color::Count;

Color Color::FILL = Color::WHITE;
Color Color::BACK = Color::BLACK;


void Color::ResetFlash()
{

}

void ColorType::Set()
{
    set.display.colors[color] = MAKE_COLOR((int)red, (int)green, (int)blue); //-V2533
}


Color Color::Channel(Channel::E chan)
{
    static const Color colors[4] = { Color::DATA_A, Color::DATA_B, Color::WHITE, Color::WHITE };
    return colors[chan];
}


Color Color::Cursors(Channel::E chan)
{
    static const Color colors[4] = { Color::DATA_A, Color::DATA_B, Color::WHITE, Color::WHITE };
    return colors[chan];
}


Color Color::Contrast(const Color &color)
{
    uint colorValue = set.display.colors[color.value];
    if (R_FROM_COLOR(colorValue) > 16 || G_FROM_COLOR(colorValue) > 32 || B_FROM_COLOR(colorValue) > 16)    //-V112
    {
        return Color::BLACK;
    }
    return Color::WHITE;
}


/*
    јлгоритм изменени€ €ркости.
    1. »нициализаци€.
    а. –ассчитать €ркость по принципу - €ркость равна относительной интенсивности самого €ркого цветового канала
    б. –ассчитать шаг изменени€ цветовой составл€ющей каждого канала на 1% €ркости по формуле:
    Ўаг = »нтенсивность канала * яркость, где яркость - относительна€ величина общей €ркости
    в. ≈сли интенсивность всех каналов == 0, то дать полный шаг каждому каналу
    2. ѕри изменнении €ркости на 1% мен€ть интенсивность каждого канала на Ўаг, расчитанный в предыдущем пункте.
    3. ѕри изменени€ интенсивности цветового канала пересчитывать €ркость и шаг изменени€ каждого канала.
*/
void ColorType::CalcSteps()
{
    stepRed = red / (brightness * 100.0F);
    stepGreen = green / (brightness * 100.0F);
    stepBlue = blue / (brightness * 100.0F);
}



void ColorType::SetBrightness()
{
    brightness = MaxFloat(red / 31.0F, green / 63.0F, blue / 31.0F);

    CalcSteps();
}



void ColorType::SetBrightness(float br)
{
    int delta = (int)((br + 0.0005F) * 100.0F) - (int)(brightness * 100.0F); //-V2533

    if (delta > 0)
    {
        for (int i = 0; i < delta; i++)
        {
            BrightnessChange(1);
        }
    }
    else
    {
        for (int i = 0; i < -delta; i++)
        {
            BrightnessChange(-1);
        }
    }
}



void ColorType::BrightnessChange(int delta) //-V2506
{
    if ((delta > 0 && brightness == 1.0F) || (delta < 0 && brightness == 0.0F)) //-V2550 //-V550
    {
        return;
    }

    int sign = Math::Sign(delta);

    LIMITATION(brightness, brightness + sign * 0.01F, 0.0F, 1.0F); //-V2516 //-V2564

    red += sign * stepRed; //-V2564
    green += sign * stepGreen; //-V2564
    blue += sign * stepBlue; //-V2564

    Set();

    if (stepRed < 0.01F && stepGreen < 0.01F && stepBlue < 0.01F)
    {
        stepRed = 0.31F;
        stepGreen = 0.63F;
        stepBlue = 0.31F;
    }
}



void ColorType::Init()
{
    if (!alreadyUsed)
    {
        alreadyUsed = true;                  // ѕризнак того, что начальные установки уже произведены

        uint colorValue = set.display.colors[color];

        red = (float)R_FROM_COLOR(colorValue);
        green = (float)G_FROM_COLOR(colorValue);
        blue = (float)B_FROM_COLOR(colorValue);

        SetBrightness();

        if (red == 0.0F && green == 0.0F && blue == 0.0F) //-V2550 //-V550
        {
            stepRed = 0.31F;
            stepGreen = 0.63F;
            stepBlue = 0.31F;
        }
    }
}



void ColorType::ComponentChange(int delta)
{
    static const float maxs[4] = {0.0F, 31.0F, 63.0F, 31.0F};
    int8 index = currentField;

    if (index >= 1 && index <= 3)
    {
        float *pointers[4] = { 0, &blue, &green, &red };
        AddLimitationFloat(pointers[index], (float)Math::Sign(delta), 0.0F, maxs[index]); //-V2533
    }

    Set();

    SetBrightness();
}
