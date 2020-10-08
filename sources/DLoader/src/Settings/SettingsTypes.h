#pragma once


// Каким курсором управлять
struct CursCntrl { enum E
{
    _1,        // первым
    _2,        // вторым
    _1_2,      // обоими
    Disable   // никаким
};};

// Дискретность перемещения курсоров
struct CursMovement { enum E
{
    Points,    // по точкам
    Percents   // по процентам
};};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursActive { enum E
{
    U,
    T,
    None
};};

// Режим слежения курсоров
struct CursLookMode { enum E
{
    None,          // Курсоры не следят
    Voltage,       // Курсоры следят за напряжением автоматически
    Time,          // Курсоры следят за временем автоматически
    Both           // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
};};


// Measures


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
