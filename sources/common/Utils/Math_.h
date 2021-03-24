// 2021/03/01 10:48:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define LIMIT_BELOW(x, min)         if(x < min) { x = min; }

#define LIMIT_ABOVE(x, max)         if(x > max) { x = max; }

#define SET_MIN_IF_LESS(x, min)     if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)   if((x) > (max)) { (max) = (x); }

#define LIMITATION(var, value, min, max) \
   var = (value); if(var < (min)) { var = (min); } else if(var > (max)) { var = (max); };

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };

#define ROUND(x) (x.value += 0.5F, x.value)

#define PI 3.141592653589793F

namespace Math
{
    uint8 GetMaxFromArray(puchar data, uint first_point, uint last_point);

    uint8 GetMinFromArray(puchar data, uint first_point, uint last_point);

    // ���������� ���������� x ����������� �����, ���������� ����� (x0, y0), (x1, y1), � �������������� ������,
    // ���������� ����� ����� � ��������� yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int y_hor_line);

    // ���������� ��� �����. ���������� true, ���� ����� ���������� ���� �� ����� �� �����, ��� �� epsilon_part.
    // ��� ���� ��� ������� epsilon_part ������������ ������� � ������ ������ �����.
    bool FloatsIsEquals(float val1, float val2, float epsilon_part);

    float RandFloat(float min, float max);

    int Sign(int vlaue);

    // ��������� 10**pow.
    int Pow10(int pow);

    // ��������� ����� �������� � ����� ���� int.
    int NumDigitsInNumber(int value);

    // ���������� ������ value.
    int FabsInt(int value);

    uint8 CalculateFiltr(puchar data, int x, int num_points, int num_smoothing);

    void CalculateFiltrArray(puchar data_in, uint8* data_out, int num_points, int num_smoothing);

    // ���������� ������� ���������� ������ �� ������� value0 � value1
    template<class T> void Swap(T *value0, T *value1);
    template<class T> void Limitation(T *value, T min, T max);
    template<class T> T MinFrom2(T val1, T val2);
    template<class T> T Limitation(T val, T min, T max);
    template<class T> void Sort(T *value1, T *value2);
    template<class T> T Max(T value1, T value2, T value3);
    // ����������� �������� �� ������ val �� 1. �����, ���� ��������� ��������� max, ������������ ��� � min
    template<class T> T CircleIncrease(T *val, T min, T max);
    // �������� �������� �� ������ val �� 1. �����, ���� ��������� ������ min, ������������ ��� max
    template<class T> T CircleDecrease(T *val, T min, T max);

    extern float Pi;
}
