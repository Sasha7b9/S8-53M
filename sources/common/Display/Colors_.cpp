#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Utils/Math_.h"
#include "Settings/Settings.h"


const Color Color::BLACK(0);
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

Color Color::FLASH_10(Color::Count + 1);
Color Color::FLASH_01(Color::Count + 2);

Color Color::current = Color::WHITE;

Color Color::FILL(Color::WHITE.index);
Color Color::BACK(Color::BLACK.index);


void Color::ResetFlash()
{

}

void ColorType::Set()
{
    set.display.colors[color] = Color::Make((uint8)red, (uint8)green, (uint8)blue);
}


Color Channel::GetColor() const
{
    static const Color colors[4] = { Color::DATA_A, Color::DATA_B, Color::WHITE, Color::WHITE };
    return colors[value];
}


Color Color::Cursors(const Channel &ch)
{
    static const Color colors[4] = { Color::DATA_A, Color::DATA_B, Color::WHITE, Color::WHITE };
    return colors[ch];
}


Color Color::Contrast(const Color &color)
{
    uint colorValue = set.display.colors[color.index];
    if (R_FROM_COLOR(colorValue) > 128 || G_FROM_COLOR(colorValue) > 128 || B_FROM_COLOR(colorValue) > 128)    //-V112
    {
        return Color::BLACK;
    }
    return Color::WHITE;
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
void ColorType::CalcSteps()
{
    stepRed = red / (brightness * 100.0F);
    stepGreen = green / (brightness * 100.0F);
    stepBlue = blue / (brightness * 100.0F);
}



void ColorType::SetBrightness()
{
    brightness = Math::Max(red / 31.0F, green / 63.0F, blue / 31.0F);

    CalcSteps();
}



void ColorType::SetBrightness(float br)
{
    int delta = (int)((br + 0.0005F) * 100.0F) - (int)(brightness * 100.0F);

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



void ColorType::BrightnessChange(int delta)
{
    if ((delta > 0 && brightness == 1.0F) || (delta < 0 && brightness == 0.0F))
    {
        return;
    }

    int sign = Math::Sign(delta);

    brightness = Math::Limitation(brightness + sign * 0.01F, 0.0F, 1.0F);

    red += sign * stepRed;
    green += sign * stepGreen;
    blue += sign * stepBlue;

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
        alreadyUsed = true;                  // ������� ����, ��� ��������� ��������� ��� �����������

        uint colorValue = set.display.colors[color];

        red = (float)R_FROM_COLOR(colorValue);
        green = (float)G_FROM_COLOR(colorValue);
        blue = (float)B_FROM_COLOR(colorValue);

        SetBrightness();

        if (red == 0.0F && green == 0.0F && blue == 0.0F)
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
        Math::AddLimitation(pointers[index], (float)Math::Sign(delta), 0.0F, maxs[index]);
    }

    Set();

    SetBrightness();
}
