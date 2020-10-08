#pragma once


// Сжимать ли сигналы при выводе измерений.
struct ModeViewSignals { enum E
{
    AsIs,       // Показывать сигналы как есть
    Compress    // Сжимать сетку с сигналами
};};


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


// Debug


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADCtype { enum E
{
    Disable,     // Балансировка выключена.
    Settings,    // Используются значения балансировки, которые получены автоматически.
    Hand         // Используются значения балансировки, заданные вручную.
};};
