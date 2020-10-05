#pragma once


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

// Масштаб по напряжению.
struct Range { enum E
{
    _2mV,
    _5mV,
    _10mV,
    _20mV,
    _50mV,
    _100mV,
    _200mV,
    _500mV,
    _1V,
    _2V,
    _5V,
    _10V,
    _20V,
    Count
};};

Range::E& operator++(Range::E &range);
Range::E& operator--(Range::E &range);

#define NumChannels 2

const char *RangeName(Range::E range);



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
struct ModeViewSignals { enum E
{
    AsIs,       // Показывать сигналы как есть
    Compress    // Сжимать сетку с сигналами
};};


// Math


struct ScaleMath { enum E
{
    Channel0,     // Масштаб берётся из канала 1
    Channel1,     // Масштаб берётся из канала 2
    Hand          // Масштаб задаётся вручную.
};};


// Курсоры

// Каким курсором управлять.
struct CursCntrl { enum E
{
    _1,            // первым.
    _2,            // вторым.
    _1_2,          // обоими.
    Disable        // никаким.
};};


// Дискретность перемещения курсоров.
struct CursMovement { enum E
{
    Points,    // по точкам.
    Percents   // по процентам.
};};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursActive { enum E
{
    U,
    T,
    None
};};

// Режим слежения курсоров.
struct CursLookMode { enum E
{
    None,      // Курсоры не следят.
    Voltage,   // Курсоры следят за напряжением автоматически.
    Time,      // Курсоры следят за временем автоматически.
    Both       // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
};};


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADCtype { enum E
{
    Disable,     // Балансировка выключена.
    Settings,    // Используются значения балансировки, которые получены автоматически.
    Hand         // Используются значения балансировки, заданные вручную.
};};

// Тип растяжки АЦП
struct StretchADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};

// Тип дополнительного смещения
struct RShiftADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};
