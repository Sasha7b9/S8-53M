// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors_.h"

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
}


Color Color::Contrast(const Color &color)
{
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
}



void ColorType::Init()
{
}



void ColorType::ComponentChange(int delta)
{
}

void Color::SetAsCurrent() const
{
    if (index != Count)
    {
        current = index;
    }
}
