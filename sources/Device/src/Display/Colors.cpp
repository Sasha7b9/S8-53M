#include "Colors.h"
#include "Settings/Settings.h"
#include "common/Log.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <cmath>



static void SetColor(ColorType *colorType)
{
    set.display.colors[colorType->color] = MAKE_COLOR((int)colorType->red, (int)colorType->green, (int)colorType->blue);
    Painter::SetPalette(colorType->color);
}


Color::E ColorChannel(Channel chan)
{
    static const Color::E colors[4] = { Color::DATA_A, Color::DATA_B, Color::WHITE, Color::WHITE };
    return colors[chan];
}


Color::E ColorCursors(Channel chan)
{
    static const Color::E colors[4] = { Color::DATA_A, Color::DATA_B, Color::WHITE, Color::WHITE };
    return colors[chan];
}


Color::E ColorTrig()
{
    TrigSource trigChan = TRIG_SOURCE;
    if (trigChan == TrigSource_ChannelA || trigChan == TrigSource_ChannelB)
    {
        return ColorChannel((Channel)trigChan);
    }
    return COLOR_FILL;
}


Color::E ColorMenuTitle(bool inShade)
{
    return inShade ? Color::MENU_ITEM : Color::MENU_TITLE;
}


Color::E ColorMenuItem(bool inShade)
{
    return inShade ? Color::MENU_ITEM_DARK : Color::MENU_ITEM;
}



Color::E ColorBorderMenu(bool inShade)
{
    return ColorMenuTitle(inShade);
}



Color::E ColorContrast(Color::E color)
{
    uint16 colorValue = set.display.colors[color];
    if (R_FROM_COLOR(colorValue) > 16 || G_FROM_COLOR(colorValue) > 32 || B_FROM_COLOR(colorValue) > 16)    //-V112
    {
        return Color::BLACK;
    }
    return Color::WHITE;
}



Color::E LightShadingTextColor()
{
    return ColorMenuTitle(false);
}



void Color_Log(Color::E color)
{
    uint16 colorValue = set.display.colors[color];
    LOG_WRITE("%s   r=%d, g=%d, b=%d", NameColor(color), R_FROM_COLOR(colorValue), G_FROM_COLOR(colorValue), B_FROM_COLOR(colorValue));
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



static void ColorType_CalcSteps(ColorType *colorType)
{
    colorType->stepRed = colorType->red / (colorType->brightness * 100.0F);
    colorType->stepGreen = colorType->green / (colorType->brightness * 100.0F);
    colorType->stepBlue = colorType->blue / (colorType->brightness * 100.0F);
}



static void ColorType_SetBrightness(ColorType *colorType)
{
    colorType->brightness = MaxFloat(colorType->red / 31.0f, colorType->green / 63.0f, colorType->blue / 31.0f);

    ColorType_CalcSteps(colorType);
}



void Color_SetBrightness(ColorType *colorType, float brightness)
{
    int delta = (int)((brightness + 0.0005f) * 100.0F) - (int)(colorType->brightness * 100.0F);

    if (delta > 0)
    {
        for (int i = 0; i < delta; i++)
        {
            Color_BrightnessChange(colorType, 1);
        }
    }
    else
    {
        for (int i = 0; i < -delta; i++)
        {
            Color_BrightnessChange(colorType, -1);
        }
    }
}



void Color_BrightnessChange(ColorType *colorType, int delta)
{
    if ((delta > 0 && colorType->brightness == 1.0f) || (delta < 0 && colorType->brightness == 0.0F))
    {
        return;
    }

    int sign = Math_Sign(delta);

    LIMITATION(colorType->brightness, colorType->brightness + sign * 0.01f, 0.0F, 1.0f);

    colorType->red += sign * colorType->stepRed;
    colorType->green += sign * colorType->stepGreen;
    colorType->blue += sign * colorType->stepBlue;

    SetColor(colorType);

    if (colorType->stepRed < 0.01f && colorType->stepGreen < 0.01f && colorType->stepBlue < 0.01f)
    {
        colorType->stepRed = 0.31f;
        colorType->stepGreen = 0.63f;
        colorType->stepBlue = 0.31f;
    }
}



void Color_Init(ColorType *colorType)
{
    if (!colorType->alreadyUsed)
    {
        colorType->alreadyUsed = true;                  // ѕризнак того, что начальные установки уже произведены

        uint16 colorValue = set.display.colors[colorType->color];

        colorType->red = (float)R_FROM_COLOR(colorValue);
        colorType->green = (float)G_FROM_COLOR(colorValue);
        colorType->blue = (float)B_FROM_COLOR(colorValue);

        ColorType_SetBrightness(colorType);

        if (colorType->red == 0.0F && colorType->green == 0.0F && colorType->blue == 0.0F)
        {
            colorType->stepRed = 0.31f;
            colorType->stepGreen = 0.63f;
            colorType->stepBlue = 0.31f;
        }
    }
}



void Color_ComponentChange(ColorType *colorType, int delta)
{
    static const float maxs[4] = {0.0F, 31.0f, 63.0f, 31.0f};
    float *pointers[4] = {0, &colorType->blue, &colorType->green, &colorType->red};
    int8 index = colorType->currentField;

    if (index >= 1 && index <= 3)
    {
        AddLimitationFloat(pointers[index], (float)Math_Sign(delta), 0.0F, maxs[index]);
    }

    SetColor(colorType);

    ColorType_SetBrightness(colorType);
}



static const char* colorNames[] =
{
    "Color::BLACK",
    "Color::WHITE",
    "Color::GRID",
    "Color::DATA_A",
    "COLOR_DATA_B",
    "Color::MENU_FIELD",
    "COLOR_MENU_TITLE",
    "COLOM_MENU_TITLE_DARK",
    "Color::MENU_TITLE_BRIGHT",
    "COLOR_MENU_ITEM",
    "Color::MENU_ITEM_DARK",
    "Color::MENU_ITEM_BRIGHT",
    "Color::MENU_SHADOW",
    "Color::NUM",
    "Color::FLASH_10",
    "Color::FLASH_01",
    "SET_INVERSE"
};



const char* NameColorFromValue(uint16 colorValue)
{
    for (int i = 0; i < Color::NUM; i++)
    {
        if (set.display.colors[(Color::E)i] == colorValue)
        {
            return colorNames[i];
        }
    }
    return "Sorry, this color is not in the palette";
}



const char* NameColor(Color::E color)
{
    return colorNames[color];
}
