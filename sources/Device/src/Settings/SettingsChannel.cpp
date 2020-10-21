#include "defines.h"
#include "common/Utils/Math_c.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"


// Структура для описания диапазона масштаба по напряжению.
struct RangeStruct
{
    const char* name[2][2];     // Название диапазона в текстовом виде, пригодном для вывода на экран.
};


// Массив структур описаний масштабов по напряжению.
static const RangeStruct ranges[Range::Count] =
{
    {"2\x10мВ",     "20\x10мВ", "2\x10mV",  "20\x10mV"},
    {"5\x10мВ",     "50\x10мВ", "5\x10mV",  "50\x10mV"},
    {"10\x10мВ",    "0.1\x10В", "10\x10mV", "0.1\x10V"},
    {"20\x10мВ",    "0.2\x10В", "20\x10mV", "0.2\x10V"},
    {"50\x10мВ",    "0.5\x10В", "50\x10mV", "0.5\x10V"},
    {"0.1\x10В",    "1\x10В",   "0.1\x10V", "1\x10V"},
    {"0.2\x10В",    "2\x10В",   "0.2\x10V", "2\x10V"},
    {"0.5\x10В",    "5\x10В",   "0.5\x10V", "5\x10V"},
    {"1\x10В",      "10\x10В",  "1\x10V",   "10\x10V"},
    {"2\x10В",      "20\x10В",  "2\x10V",   "20\x10V"},
    {"5\x10В",      "50\x10В",  "5\x10V",   "50\x10V"},
    {"10\x10В",     "100\x10В", "10\x10V",  "100\x10V"},
    {"20\x10В",     "200\x10В", "20\x10V",  "200\x10V"}
};



void sChannel_SetRange(Channel::E chan, Range::E range)
{
    SET_RANGE(chan) = range;
}


bool sChannel_Enabled(Channel::E chan)
{
    if (chan == Channel::Math)
    {
        return !DISABLED_DRAW_MATH;
    }
    if (chan == Channel::A_B)
    {
        return false;
    }
    return SET_ENABLED(chan);
}


const char* sChannel_Range2String(Range::E range, Divider::E multiplier)
{
    return ranges[range].name[LANG][multiplier];
}


const char* sChannel_RShift2String(int16 rShiftRel, Range::E range, Divider::E multiplier, char buffer[20])
{
    float rShiftVal = RSHIFT_2_ABS(rShiftRel, range) * Divider::ToAbs(multiplier);
    return Voltage2String(rShiftVal, true, buffer);
};
