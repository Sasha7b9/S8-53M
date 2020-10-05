#pragma once


// Делитель.
struct Divider { enum E
    {
        _1,
        _10
    };
    static int ToAbs(Divider::E divider);
};


// Тип выборки для режима рандомизатора.
struct SampleType { enum E
{
    Real,   // реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается.
    Equal   // эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора.
};};


// Число точек сигнала, с которым идёт работа.
struct ENUM_POINTS_FPGA { enum E
{
    _281,
    _512,
    _1024
};};


// Источинк сигнала для отображения спектра.
struct SourceFFT { enum E
{
    A,          // Расчёт и отображение спектра первого канала.
    B,          // Расчёт и отображение спектра второго канала.
    Both        // Расчёт и отображение спектров обоих каналов.
};};


// Язык меню
struct Language { enum E
{
    Russian,    // Русский
    English     // Английский
};};


// Тип растяжки АЦП
struct StretchADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};


// Режим отрисовки сигнала.
struct ModeDrawSignal {enum E
{
    Lines,      // Сигнал рисуется линиями.
    Points      // Сигнал рисуется точками.
};};


// Тип сетки на экране.
struct TypeGrid { enum E
{
    _1,
    _2,
    _3,
    _4,
    Count
};};


// Цветовая схема
struct ColorScheme { enum E
{
    WhiteLetters,   // В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу.
    BlackLetters    // В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит.
};};


// Перечисление накоплений.
struct ENumAccumulation { enum E
{
    _1,
    _2,
    _4,
    _8,
    _16,
    _32,
    _64,
    _128,
    Infinity
};};


// Количество усреднений по измерениям.
struct ENumAveraging { enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        _512
    };

    static int NumAverages();
};


// Тип усреднений по измерениям.
struct ModeAveraging { enum E
    {
        Accurately,   // Усреднять точно.
        Around        // Усреднять приблизительно.
    };

    // Возвращает режим усреднения
    static ModeAveraging::E Current();
};
