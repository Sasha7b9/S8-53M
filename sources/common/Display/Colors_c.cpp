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

static void SetColor(const ColorType *colorType)
{
    set.display.colors[colorType->color] = MAKE_COLOR((int)colorType->red, (int)colorType->green, (int)colorType->blue); //-V2533
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


void Color_Log(Color color)
{
    uint colorValue = set.display.colors[color.value];
    LOG_WRITE("%s   r=%d, g=%d, b=%d", NameColor(color), R_FROM_COLOR(colorValue), G_FROM_COLOR(colorValue), B_FROM_COLOR(colorValue));
}


/*
    �������� ��������� �������.
    1. �������������.
    �. ���������� ������� �� �������� - ������� ����� ������������� ������������� ������ ������ ��������� ������
    �. ���������� ��� ��������� �������� ������������ ������� ������ �� 1% ������� �� �������:
    ��� = ������������� ������ * �������, ��� ������� - ������������� �������� ����� �������
    �. ���� ������������� ���� ������� == 0, �� ���� ������ ��� ������� ������
    2. ��� ���������� ������� �� 1% ������ ������������� ������� ������ �� ���, ����������� � ���������� ������.
    3. ��� ��������� ������������� ��������� ������ ������������� ������� � ��� ��������� ������� ������.
*/
static void ColorType_CalcSteps(ColorType *colorType)
{
    colorType->stepRed = colorType->red / (colorType->brightness * 100.0F);
    colorType->stepGreen = colorType->green / (colorType->brightness * 100.0F);
    colorType->stepBlue = colorType->blue / (colorType->brightness * 100.0F);
}



static void ColorType_SetBrightness(ColorType *colorType)
{
    colorType->brightness = MaxFloat(colorType->red / 31.0F, colorType->green / 63.0F, colorType->blue / 31.0F);

    ColorType_CalcSteps(colorType);
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

    SetColor(this);

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
        alreadyUsed = true;                  // ������� ����, ��� ��������� ��������� ��� �����������

        uint colorValue = set.display.colors[color];

        red = (float)R_FROM_COLOR(colorValue);
        green = (float)G_FROM_COLOR(colorValue);
        blue = (float)B_FROM_COLOR(colorValue);

        ColorType_SetBrightness(this);

        if (red == 0.0F && green == 0.0F && blue == 0.0F) //-V2550 //-V550
        {
            stepRed = 0.31F;
            stepGreen = 0.63F;
            stepBlue = 0.31F;
        }
    }
}



void Color_ComponentChange(ColorType *colorType, int delta)
{
    static const float maxs[4] = {0.0F, 31.0F, 63.0F, 31.0F};
    int8 index = colorType->currentField;

    if (index >= 1 && index <= 3)
    {
        float *pointers[4] = { 0, &colorType->blue, &colorType->green, &colorType->red };
        AddLimitationFloat(pointers[index], (float)Math::Sign(delta), 0.0F, maxs[index]); //-V2533
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
    "Color::Count",
    "Color::FLASH_10",
    "Color::FLASH_01",
    "SET_INVERSE"
};



const char* NameColor(const Color &color)
{
    return colorNames[color.value];
}
