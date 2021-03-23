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
    (((float)(value) - (float)Value::MIN) * MathFPGA::voltsInPixel[(range)] -     \
    MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define TSHIFT_2_ABS(tShift, tBase)  (MathFPGA::absStepTShift[(tBase)] * (tShift) * 2.0F)

#define TSHIFT_2_REL(tShiftAbs, tBase) ((tShiftAbs) / MathFPGA::absStepTShift[(tBase)] / 2.0F)

#define ROUND(x) (x.value += 0.5F, x.value)

#define PI 3.141592653589793F

namespace Math
{
    uint8 GetMaxFromArray(puchar data, uint first_point, uint last_point);

    uint8 GetMinFromArray(puchar data, uint first_point, uint last_point);

    // ¬озвращает координату x пересечени€ линии, проход€щей через (x0, y0), (x1, y1), с горизонтальной линией,
    // проход€щей через точку с ординатой yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int y_hor_line);

    // —равнивает два числа. ¬озвращает true, если числа отличаютс€ друг от друга не более, чем на epsilon_part.
    // ѕри этом дл€ расчЄта epsilon_part используетс€ большее в смысле модул€ число.
    bool FloatsIsEquals(float val1, float val2, float epsilon_part);

    float RandFloat(float min, float max);

    int Sign(int vlaue);

    // ¬ычисл€ет 10**pow.
    int Pow10(int pow);

    // ¬ычисл€ет число разр€дов в целом типа int.
    int NumDigitsInNumber(int value);

    // ¬озвращает модуль value.
    int FabsInt(int value);

    uint8 CalculateFiltr(puchar data, int x, int num_points, int num_smoothing);

    void CalculateFiltrArray(puchar data_in, uint8* data_out, int num_points, int num_smoothing);

    // ќбменивает местами содержимое пам€ти по адресам value0 и value1
    template<class T> void Swap(T *value0, T *value1);
    template<class T> void Limitation(T *value, T min, T max);
    template<class T> T MinFrom2(T val1, T val2);
    template<class T> T Limitation(T val, T min, T max);
    template<class T> void Sort(T *value1, T *value2);

    extern float Pi;
}
