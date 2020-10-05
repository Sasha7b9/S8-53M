#pragma once



// SDisplay


// Режим отображения пропущенных сигналов при ограничении частоты кадров.
enum MissedSignals
{
    Missed_Hide,    // Не выводить на экран
    Missed_Show,    // Выводить на экран
    Missed_Average  // Устреднять и выводить на экран
};



// ChannelX

struct Channel { enum E
{
    A,
    B,
    A_B,
    Math
};};

// Режим канала по входу.
struct ModeCouple { enum E
{
    DC,      // Открытый вход.
    AC,      // Закрытый вход.
    GND      // Вход заземлён.
};};

// Делитель.
enum Divider
{
    Divider_1,
    Divider_10
};

// Масштаб по напряжению.
enum Range
{
    Range_2mV,
    Range_5mV,
    Range_10mV,
    Range_20mV,
    Range_50mV,
    Range_100mV,
    Range_200mV,
    Range_500mV,
    Range_1V,
    Range_2V,
    Range_5V,
    Range_10V,
    Range_20V,
    RangeSize
};

Range& operator++(Range &range);
Range& operator--(Range &range);

#define NumChannels 2

const char *RangeName(Range range);



// Time


// Масштаб по времени
struct TBase { enum E
{
    _2ns,
    _5ns,
    _10ns,
    _20ns,
    _50ns,
    _100ns,
    _200ns,
    _500ns,
    _1us,
    _2us,
    _5us,
    _10us,
    _20us,
    _50us,
    _100us,
    _200us,
    _500us,
    _1ms,
    _2ms,
    _5ms,
    _10ms,
    _20ms,
    _50ms,
    _100ms,
    _200ms,
    _500ms,
    _1s,
    _2s,
    _5s,
    _10s,
    Count
}; };

const char *TBaseName(TBase::E tBase);



// Measures


// Сжимать ли сигналы при выводе измерений.
enum ModeViewSignals
{
    ModeViewSignals_AsIs,       // Показывать сигналы как есть
    ModeViewSignals_Compress    // Сжимать сетку с сигналами
};


// Math


enum ScaleMath
{
    ScaleMath_Channel0,     // Масштаб берётся из канала 1
    ScaleMath_Channel1,     // Масштаб берётся из канала 2
    ScaleMath_Hand          // Масштаб задаётся вручную.
};


// Курсоры

// Каким курсором управлять.
enum CursCntrl
{
    CursCntrl_1,            // первым.
    CursCntrl_2,            // вторым.
    CursCntrl_1_2,          // обоими.
    CursCntrl_Disable       // никаким.
};


// Дискретность перемещения курсоров.
enum CursMovement
{
    CursMovement_Points,    // по точкам.
    CursMovement_Percents   // по процентам.
};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
enum CursActive
{
    CursActive_U,
    CursActive_T,
    CursActive_None
};

// Режим слежения курсоров.
enum CursLookMode
{
    CursLookMode_None,      // Курсоры не следят.
    CursLookMode_Voltage,   // Курсоры следят за напряжением автоматически.
    CursLookMode_Time,      // Курсоры следят за временем автоматически.
    CursLookMode_Both       // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
};


// Service


// Язык меню
enum Language
{
    Russian,    // Русский
    English     // Английский
};


// Debug


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
enum BalanceADCtype
{
    BalanceADC_Disable,     // Балансировка выключена.
    BalanceADC_Settings,    // Используются значения балансировки, которые получены автоматически.
    BalanceADC_Hand         // Используются значения балансировки, заданные вручную.
};

// Тип растяжки АЦП
enum StretchADCtype
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
};

// Тип дополнительного смещения
enum RShiftADCtype
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
};
