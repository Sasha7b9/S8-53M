// 2021/07/02 9:35:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Animated_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"


ABorder::ABorder(int _w, int _h) : width(_w), height(_h)
{

}


void ABorder::Draw()
{
    static const int numPoints = GetPerimeter() / 10;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(TIME_MS / 20);

        BitSet64 coord = CalculateCoord(value + i * (GetPerimeter() / numPoints));

        Point().Draw(coord.first, coord.second, Color::FILL);
    }
}


BitSet64 ABorder::CalculateCoord(int value)
{
    value %= GetPerimeter();

    if (value < width)
    {
        return BitSet64((int)value, 0);
    }
    else if (value < width + height)
    {
        return BitSet64(width - 1, (int)value - width);
    }
    else if (value < width * 2 + height)
    {
        return BitSet64(width - ((int)value - width - height), height - 1);
    }

    return BitSet64(0, height - ((int)value - width * 2 - height));
}


int ABorder::GetPerimeter()
{
    return width * 2 + height * 2;
}

