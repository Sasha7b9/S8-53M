#include "defines.h"
#include "common/Utils/Math_c.h"
#include "Settings/Settings.h"
#include <cstdlib>
#include <cstring>
#include <cmath>


int Math::MinFrom2Int(int val0, int val1) //-V2506
{
    if (val0 < val1)
    {
        return val0;
    }
    return val1;
}


float Math::GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine) //-V2506
{
    if(y0 == y1)
    {
        return static_cast<float>(x1);
    }

    return (yHorLine - y0) / ((float)(y1 - y0) / (float)(x1 - x0)) + x0; //-V2533 //-V2564
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


int Math::MinInt(int val1, int val2)
{
    return val1 < val2 ? val1 : val2;
}


#define SET_MIN_MAX(x)		\
	if (x < min) min = x;	\
	if (x > max) max = x;


float Math::RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((std::rand() / (float)RAND_MAX) * delta); //-V2564
}


int8 Math_AddInt8WithLimitation(int8 value, int8 delta, int8, int8 max) //-V2506
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


int Math::Sign(int value) //-V2506
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
    while ((value /= 10) > 0) //-V2561
    {
        num++;
    }
    return num;
}


int Math::FabsInt(int value)
{
    return value >= 0 ? value : -value;
}


uint8 Math::GetMinFromArray(pUCHAR data, int firstPoint, int lastPoint)
{

#define MIN_IF_LESS if(d < min) { min = d; }

    uint8 min = 255;
    pUCHAR pointer = &data[firstPoint]; //-V2563

    for (int i = firstPoint; i < lastPoint; i += 2)
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


uint8 Math::GetMaxFromArray(pUCHAR data, int firstPoint, int lastPoint)
{

#define MAX_IF_ABOVE if(d > max) { max = d; }

    uint8 max = 0;
    pUCHAR pointer = &data[firstPoint]; //-V2563

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MAX_IF_ABOVE;
        d = *pointer++;
        MAX_IF_ABOVE;
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MAX_IF_ABOVE
    }

    return max;
}

#define LIMIT                           \
    if (value <= min) { return min; }   \
    if (value >= max) { return max; }   \
    return value;

int LimitationInt(int value, int min, int max) //-V2506
{
    LIMIT
}

uint8 LimitationUInt8(uint8 value, uint8 min, uint8 max) //-V2506
{
    LIMIT
}

float LimitationFloat(float value, float min, float max) //-V2506
{
    LIMIT
}

uint8 Math::CalculateFiltr(pUCHAR data, int x, int numPoints, int numSmoothing) //-V2506
{
    if (numSmoothing < 2)
    {
        return data[x]; //-V2563
    }

    int count = 1;
    int sum = data[x]; //-V2563
    int startDelta = 1;

    int endDelta = numSmoothing / 2;

    for (int delta = startDelta; delta <= endDelta; delta++)
    {
        if (((x - delta) >= 0) && ((x + delta) < (numPoints)))
        {
            sum += data[x - delta]; //-V2563
            sum += data[x + delta]; //-V2563
            count += 2;
        }
    }

    if ((numSmoothing % 2) == 1)
    {
        int delta = numSmoothing / 2;
        if ((x + delta) < numPoints)
        {
            sum += data[x + delta]; //-V2563
            count++;
        }
    }

    return (uint8)(sum / count); //-V2533
}

void Math::CalculateFiltrArray(pUCHAR dataIn, uint8 *dataOut, int numPoints, int numSmoothing)
{
    if (numSmoothing < 2)
    {
        std::memcpy(dataOut, dataIn, static_cast<uint>(numPoints));
    }
    else
    {
        bool addCalculation = (numSmoothing % 2) == 1;
        int endDelta = numSmoothing / 2;
        int startDelta = 1;
        int d = numSmoothing / 2;

        for (int i = 0; i < numPoints; i++)
        {
            int count = 1;
            int sum = dataIn[i]; //-V2563

            for (int delta = startDelta; delta <= endDelta; delta++)
            {
                if (((i - delta) >= 0) && ((i + delta) < (numPoints)))
                {
                    sum += dataIn[i - delta]; //-V2563
                    sum += dataIn[i + delta]; //-V2563
                    count += 2;
                }
            }

            if (addCalculation)
            {
                if ((i + d) < numPoints)
                {
                    sum += dataIn[i + d]; //-V2563
                    count++;
                }
            }

            dataOut[i] = (uint8)(sum / count); //-V2533 //-V2563
        }
    }
}
