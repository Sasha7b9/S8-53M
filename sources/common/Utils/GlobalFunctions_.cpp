#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Containers/Values_.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>


int GF::NumDigitsInIntPart(float value)
{
    float fabsValue = std::fabsf(value);

    int numDigitsInInt = 0;
    if (fabsValue >= 10000)
    {
        numDigitsInInt = 5;
    }
    else if (fabsValue >= 1000)
    {
        numDigitsInInt = 4;
    }
    else if (fabsValue >= 100)
    {
        numDigitsInInt = 3;
    }
    else if (fabsValue >= 10)
    {
        numDigitsInInt = 2;
    }
    else
    {
        numDigitsInInt = 1;
    }

    return numDigitsInInt;
}


String GF::LogArrayUint8(uint8 *array, uint size)
{
    String result;

    for (uint i = 0; i < size; i++)
    {
        result.Append(Int(array[i]).ToString(false, 1).c_str());
        
        if (i != size - 1)
        {
            result.Append(' ');
        }
    }

    return result;
}
