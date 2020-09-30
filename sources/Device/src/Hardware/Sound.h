#pragma once


enum TypeWave
{
    TypeWave_Sine,
    TypeWave_Meandr,
    TypeWave_Triangle
};


class Sound
{
public:

    static void ButtonPress();
    // Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    static void ButtonRelease();

    static void GovernorChangedValue();

    static void RegulatorShiftRotate();

    static void RegulatorSwitchRotate();

    static void WarnBeepBad();

    static void WarnBeepGood();
};
