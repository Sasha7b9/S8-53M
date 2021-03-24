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


String GF::FloatFract2String(float value, bool alwaysSign)
{
    return GF::Float2String(value, alwaysSign, 4);
}

static int NumDigitsInIntPart(float value)
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

String GF::Float2String(const Float &value, bool always_sign, int num_digits)
{
    if(!value.IsValid())
    {
        return EmptyString();
    }

    String result;

    if(!always_sign)
    {
        if(value < 0.0f)
        {
            result.Append('-');
        }
    }
    else
    {
        result.Append(value < 0.0f ? '-' : '+');
    }


    char buffer[20];
    char format[] = "%4.2f\0\0";

    format[1] = (char)num_digits + 0x30;

    int num_digits_in_int = NumDigitsInIntPart(value);

    format[3] = (char)((num_digits - num_digits_in_int) + 0x30);
    if(num_digits == num_digits_in_int)
    {
        format[5] = '.';
    }
    
    std::sprintf(buffer, format, std::fabsf(value));

    result.Append(buffer);

    float val = (float)(std::atof(buffer)); //-V2508

    if (NumDigitsInIntPart(val) != num_digits_in_int)
    {
        num_digits_in_int = NumDigitsInIntPart(val);
        format[3] = (char)((num_digits - num_digits_in_int) + 0x30);

        if (num_digits == num_digits_in_int)
        {
            format[5] = '.';
        }

        std::sprintf(buffer, format, value.value);

        result.Append(buffer);
    }

    bool signExist = always_sign || value < 0.0f;

    while((int)(result.Size()) < num_digits + (signExist ? 2 : 1))
    {
        result.Append("0");
    }

    return result;
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
    return String("%säÁ", GF::Float2String(value, false, num_digits).c_str());
}


void GF::IntToStrCat(char *_buffer, int _value)
{
    static const int LENGHT = 10;
    char buffer[LENGHT];
    for(int i = 0; i < LENGHT; i++)
    {
        buffer[i] = 0;
    }
    int pointer = LENGHT -1;

    while(_value > 0)
    {
        buffer[pointer] = (int8)(_value % 10);
        _value /= 10;
        pointer--;
    }

    while(*_buffer)
    {
        _buffer++;
    }
    int i = 0;

    for(; i < LENGHT; i++)
    {
        if(buffer[i] > 0)
        {
            break;
        }
    }

    for(; i < LENGHT; i++)
    {
        *_buffer = 0x30 | buffer[i];
        _buffer++;
    }

    *_buffer = 0;
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
