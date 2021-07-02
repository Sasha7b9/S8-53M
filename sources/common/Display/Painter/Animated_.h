// 2021/07/02 9:34:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ABorder
{
public:

    ABorder(int width, int height);

    void Draw();

    int GetPerimeter();

    BitSet64 CalculateCoord(int value);

private:

    int width;
    int height;
};
