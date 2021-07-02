// 2021/07/02 9:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Animated_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"


ABorder::ABorder() : startTime(TIME_MS)
{

}


void ABorder::Draw()
{
    static const int numPoints = GetPerimeter() / 10;

    for (int i = 0; i < numPoints; i++)
    {
        uint value = (TIME_MS - startTime) / 20;

        BitSet64 coord = CalculateCoord(value + i * (GetPerimeter() / numPoints));

        Point().Draw(coord.first, coord.second, Color::FILL);
    }

    Rectangle(Display::WIDTH - 2, Display::HEIGHT - 2).Draw(1, 1);
}


BitSet64 ABorder::CalculateCoord(uint value)
{
    value %= GetPerimeter();

    if (value < Display::WIDTH)
    {
        return BitSet64((int)value, 0);
    }
    else if (value < Display::WIDTH + Display::HEIGHT)
    {
        return BitSet64(Display::WIDTH - 1, (int)value - Display::WIDTH);
    }
    else if (value < Display::WIDTH * 2 + Display::HEIGHT)
    {
        return BitSet64(Display::WIDTH - ((int)value - Display::WIDTH - Display::HEIGHT), Display::HEIGHT - 1);
    }

    return BitSet64(0, Display::HEIGHT - ((int)value - Display::WIDTH * 2 - Display::HEIGHT));
}


int ABorder::GetPerimeter()
{
    return Display::WIDTH * 2 + Display::HEIGHT * 2;
}

