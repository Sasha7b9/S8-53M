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


String GF::Bin2String(uint8 value)
{
    String result("00000000");

    char *buffer = result.c_str();

    for(int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    return result;
}


String GF::Bin2String16(uint16 value)
{
    String result("000000000000000000");

    std::strcpy(&result[0], Bin2String((uint8)(value >> 8)).c_str());

    result.c_str()[8] = ' ';

    std::strcpy(&result[9], Bin2String((uint8)value).c_str());

    return result;
}


String GF::Hex8toString(uint8 value)
{
    String result("00");

    std::sprintf(&result[value < 16 ? 1U : 0U], "%x", value);

    return result;
}


String GF::Float2Db(float value, int num_digits)
{
    return String("%säÁ", Float(value).ToString(false, num_digits).c_str());
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
