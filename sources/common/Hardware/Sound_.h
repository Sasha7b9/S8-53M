// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TypeWave { enum E
{
    Sine,
    Meandr,
    Triangle
};};


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

    volatile static bool isBeep;
    static bool warnIsBeep;
    static bool buttonIsPressed;
};
