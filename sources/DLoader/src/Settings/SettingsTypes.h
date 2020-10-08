#pragma once


struct Function { enum E
{
    Sum,
    Mul
};};

struct ScaleFFT { enum E
{
    Log,           // Это значение означает логарифмическую шкалу вывода спектра
    Linear         // Это значение означает линейную шкалу вывода спектра
}; };

struct WindowFFT { enum E
{
    Rectangle,
    Hamming,
    Blackman,
    Hann
};};

struct FFTmaxDB { enum E
{
    _40,
    _60,
    _80
};};

struct ModeDrawMath { enum E
{
    Disable,
    Separate,
    Together
};};

struct ModeRegSet { enum E
{
    Range,
    RShift
};};


struct CalibratorMode { enum E
{
    Freq,
    DC,
    GND
};};
