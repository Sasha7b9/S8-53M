// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "Settings/Settings.h"
#include <cstdlib>
#include <cstring>
#include <cmath>


template int   Math::Abs(int);
template int16 Math::Abs(int16);
template void  Math::Swap(int *, int *);
template void  Math::Swap(uint8 *, uint8 *);
template void  Math::Limitation(int *, int, int);
template void  Math::Limitation(int16 *, int16, int16);
template void  Math::Limitation(float *, float, float);
template void  Math::LimitAbove(int16 *, int16);
template void  Math::LimitAbove(int *, int);
template void  Math::LimitBelow(int16 *, int16);
template void  Math::LimitBelow(float *, float);
template void  Math::LimitBelow(int *, int);
template int   Math::MinFrom2(int, int);
template uint  Math::MinFrom2(uint, uint);
template int   Math::Limitation(int, int, int);
template uint8 Math::Limitation(uint8, uint8, uint8);
template float Math::Limitation(float, float, float);
template int16 Math::Limitation(int16, int16, int16);
template void  Math::Sort(int *, int *);
template float Math::Max(float, float, float);
template int8  Math::CircleIncrease(int8 *, int8, int8);
template int16 Math::CircleIncrease(int16 *, int16, int16);
template int   Math::CircleIncrease(int *, int, int);
template int8  Math::CircleDecrease(int8 *, int8, int8);
template int16 Math::CircleDecrease(int16 *, int16, int16);
template int   Math::CircleDecrease(int *, int, int);
template void  Math::AddLimitation(float *, float, float, float);
template void  Math::AddLimitation(int16 *, int16, int16, int16);
template bool  Math::InRange(int, int, int);


const float Math::Pi = 3.14159265358979323846F;


template<class T>
T Math::MinFrom2(T val1, T val2)
{
    if (val1 < val2)
    {
        return val1;
    }

    return val2;
}


float Math::GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine)
{
    if(y0 == y1)
    {
        return (float)(x1);
    }

    return (yHorLine - y0) / ((float)(y1 - y0) / (float)(x1 - x0)) + x0;
}

bool Math::FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = std::fabsf(value0) > std::fabsf(value1) ? std::fabsf(value0) : std::fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return std::fabsf(value0 - value1) < epsilonAbs;
}

float Math_MinFrom3float(float value1, float value2, float value3)
{
    float retValue = value1;
    if(value2 < retValue)
    {
        retValue = value2;
    }
    if(value3 < retValue)
    {
        retValue = value3;
    }
    return retValue;
}


#define SET_MIN_MAX(x)		\
	if (x < min) min = x;	\
	if (x > max) max = x;


float Math::RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((std::rand() / (float)RAND_MAX) * delta);
}


int8 Math_AddInt8WithLimitation(int8 value, int8 delta, int8, int8 max)
{
    int8 retValue = value + delta;
    if (retValue < 0)
    {
        return 0;
    }
    if (retValue > max)
    {
        return max;
    }
    return retValue;
}


int Math::Sign(int value)
{
    if (value > 0)
    {
        return 1;
    }
    if (value < 0)
    {
        return -1;
    }
    return 0;
}


int Math::Pow10(int pow)
{
    int retValue = 1;

    while (pow--)
    {
        retValue *= 10;
    }

    return retValue;
}


int Math::NumDigitsInNumber(int value)
{
    value = Math::FabsInt(value);
    int num = 1;
    while ((value /= 10) > 0)
    {
        num++;
    }
    return num;
}


int Math::FabsInt(int value)
{
    return value >= 0 ? value : -value;
}


uint8 Math::GetMinFromArray(puchar data, uint firstPoint, uint lastPoint)
{

#define MIN_IF_LESS if(d < min) { min = d; }

    uint8 min = 255;
    puchar pointer = &data[firstPoint];

    for (uint i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MIN_IF_LESS
        d = *pointer++;
        MIN_IF_LESS
    }

    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MIN_IF_LESS
    }

    return min;
}


uint8 Math::GetMaxFromArray(puchar data, uint first_point, uint last_point)
{

#define MAX_IF_ABOVE if(d > max) { max = d; }

    uint8 max = 0;
    puchar pointer = &data[first_point];

    for (uint i = first_point; i < last_point; i += 2)
    {
        uint8 d = *pointer++;
        MAX_IF_ABOVE;
        d = *pointer++;
        MAX_IF_ABOVE;
    }
    if ((last_point - first_point + 1) & 1)
    {
        uint8 d = *pointer;
        MAX_IF_ABOVE
    }

    return max;
}


template<class T>
T Math::Limitation(T val, T min, T max)
{
    if (val <= min) { return min; }

    if (val >= max) { return max; }

    return val;
}


template<class T>
void Math::Sort(T *value1, T *value2)
{
    if (*value1 > *value2)
    {
        T temp = *value1;
        *value1 = *value2;
        *value2 = temp;
    }
}


template<class T>
T Math::Max(T value1, T value2, T value3)
{
    T result = value1;

    if (value2 > result)
    {
        result = value2;
    }

    if (value3 > result)
    {
        result = value3;
    }

    return result;
}


template<class T>
T Math::Abs(T value)
{
    return (value < (T)0) ? -value : value;
}


void Math::CalculateFiltrArray(uint8 *dataIn, uint8 *dataOut, int num_points, int numSmoothing)
{
    if (numSmoothing < 2)
    {
        std::memcpy(dataOut, dataIn, (uint)(num_points));
    }
    else
    {
        bool addCalculation = (numSmoothing % 2) == 1;
        int endDelta = numSmoothing / 2;
        int startDelta = 1;
        int d = numSmoothing / 2;

        for (int i = 0; i < num_points; i++)
        {
            int count = 1;
            int sum = dataIn[i];

            for (int delta = startDelta; delta <= endDelta; delta++)
            {
                if (((i - delta) >= 0) && ((i + delta) < (num_points)))
                {
                    sum += dataIn[i - delta];
                    sum += dataIn[i + delta];
                    count += 2;
                }
            }

            if (addCalculation)
            {
                if ((i + d) < num_points)
                {
                    sum += dataIn[i + d];
                    count++;
                }
            }

            dataOut[i] = (uint8)(sum / count);
        }
    }
}


template<class T>
void Math::Swap(T *value0, T *value1)
{
    T temp = *value0; *value0 = *value1; *value1 = temp;
}


template<class T>
void Math::Limitation(T *value, T min, T max)
{
    if(*value < min)
    {
        *value = min;
    }
    else if(*value > max)
    {
        *value = max;
    }
}


template<class T>
void Math::LimitAbove(T *value, T max)
{
    if (*value > max)
    {
        *value = max;
    }
}


template<class T>
void Math::LimitBelow(T *value, T min)
{
    if (*value < min)
    {
        *value = min;
    }
}


template<class T>
T Math::CircleIncrease(T *val, T min, T max)
{
    (*val)++;

    if ((*val) > max)
    {
        (*val = min);
    }

    return (*val);
}


template<class T>
T Math::CircleDecrease(T *val, T min, T max)
{
    (*val)--;

    if ((*val) < min)
    {
        (*val) = max;
    }

    return *val;
}


template<class T>
T Math::CircleAdd(T *val, T delta, T min, T max)
{
    *val += delta;

    if (*val > max)
    {
        *val = min;
    }

    return *val;
}


template<class T>
T Math::CircleSub(T *val, T delta, T min, T max)
{
    *val -= delta;

    if (*val < min)
    {
        *val = max;
    }

    return *val;
}


template<class T>
void Math::AddLimitation(T *val, T delta, T min, T max)
{
    T sum = *val + delta;

    if (sum < min)
    {
        *val = min;
    }
    else if (sum > max)
    {
        *val = max;
    }
    else
    {
        *val = sum;
    }
}


template<class T>
bool Math::InRange(T value, T min, T max)
{
    if (value < min)    { return false; }

    if (value > max)    { return false; }

    return true;
}


uint Math::ToUINT(const void *pointer)
{
#ifndef __linux__
    return (uint)pointer;
#else
    uint64 result = (uint64)pointer;
    return (uint)(result >> 32);
#endif
}
