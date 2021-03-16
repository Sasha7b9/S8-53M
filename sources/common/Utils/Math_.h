// 2021/03/01 10:48:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define LIMIT_BELOW(x, min)         if(x < min) { x = min; }

#define LIMIT_ABOVE(x, max)         if(x > max) { x = max; }

#define SET_MIN_IF_LESS(x, min)     if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)   if((x) > (max)) { (max) = (x); }

#define LIMITATION(var, value, min, max) \
   var = (value); if(var < (min)) { var = (min); } else if(var > (max)) { var = (max); };

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };

#define POINT_2_VOLTAGE(value, range, rShift)                                    \
    (((float)(value) - (float)MIN_VALUE) * MathFPGA::voltsInPixel[(range)] -     \
    MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define TSHIFT_2_ABS(tShift, tBase)  (MathFPGA::absStepTShift[(tBase)] * (tShift) * 2.0F)

#define TSHIFT_2_REL(tShiftAbs, tBase) ((tShiftAbs) / MathFPGA::absStepTShift[(tBase)] / 2.0F)

#define ROUND(x) (x += 0.5F, x)

#define PI 3.141592653589793F

namespace Math
{
    uint8   GetMaxFromArray(pUCHAR data, int firstPoint, int lastPoint);

    uint8   GetMinFromArray(pUCHAR data, int firstPoint, int lastPoint);

    // ¬озвращает координату x пересечени€ линии, проход€щей через (x0, y0), (x1, y1), с горизонтальной линией, проход€щей через точку с ординатой yHorLine.
    float   GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);

    // —равнивает два числа. ¬озвращает true, если числа отличаютс€ друг от друга не более, чем на epsilonPart. ѕри этом дл€ расчЄта epsilonPart 
    // используетс€ большее в смысле модул€ число.
    bool    FloatsIsEquals(float value0, float value1, float epsilonPart);

    float   RandFloat(float min, float max);

    int8    AddInt8WithLimitation(int8 value, int8 delta, int8 min, int8 max);

    int     Sign(int vlaue);

    // ¬ычисл€ет 10**pow.
    int     Pow10(int pow);

    // ¬ычисл€ет число разр€дов в целом типа int.
    int     NumDigitsInNumber(int value);

    // ¬озвращает модуль value.
    int     FabsInt(int value);

    uint8   CalculateFiltr(pUCHAR data, int x, int numPoints, int numSmoothing);

    void    CalculateFiltrArray(pUCHAR dataIn, uint8* dataOut, int numPoints, int numSmoothing);

    // ќбменивает местами содержимое пам€ти по адресам value0 и value1
    template<class T> void Swap(T *value0, T *value1);
    template<class T> void Limitation(T *value, T min, T max);
    template<class T> T MinFrom2(T val1, T val2);
}



int     LimitationInt(int value, int min, int max);
uint8   LimitationUInt8(uint8 value, uint8 min, uint8 max);
float   LimitationFloat(float value, float min, float max);

