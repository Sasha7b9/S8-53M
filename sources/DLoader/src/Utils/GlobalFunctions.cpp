#include "defines.h"
#include "GlobalFunctions.h"
#include "Settings/Settings.h"
#include "common/Utils/Math_.h"
#include <cmath>
#include <cstring>
#include <ctype.h>
#include <cstdio>
#include <cstdlib>


char* FloatFract2String(float value, bool alwaysSign, char bufferOut[20])
{
    return Float2String(value, alwaysSign, 4, bufferOut);
}



static int NumDigitsInIntPart(float value)
{
    float fabsValue = std::fabsf(value);

    int numDigitsInInt = 0;
    if (fabsValue >= 10000) //-V2564
    {
        numDigitsInInt = 5;
    }
    else if (fabsValue >= 1000) //-V2564
    {
        numDigitsInInt = 4;
    }
    else if (fabsValue >= 100) //-V2564
    {
        numDigitsInInt = 3;
    }
    else if (fabsValue >= 10) //-V2564
    {
        numDigitsInInt = 2;
    }
    else
    {
        numDigitsInInt = 1;
    }

    return numDigitsInInt;
}



char* Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char *pBuffer = bufferOut;

    if (value == ERROR_VALUE_FLOAT) //-V2550 //-V550
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE); //-V2513
        return bufferOut;
    }

    if (!alwaysSign)
    {
        if (value < 0) //-V2564
        {
            *pBuffer = '-';
            pBuffer++;
        }
    }
    else
    {
        *pBuffer = value < 0 ? '-' : '+'; //-V2564
        pBuffer++;
    }

    char format[] = "%4.2f\0\0";

    format[1] = (char)numDigits + 0x30;

    int numDigitsInInt = NumDigitsInIntPart(value);

    format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }

    std::sprintf(pBuffer, format, std::fabsf(value));

    float val = (float)std::atof(pBuffer); //-V2508

    if (NumDigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = NumDigitsInIntPart(val);
        format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        std::sprintf(pBuffer, format, value);
    }

    bool signExist = alwaysSign || value < 0; //-V2564
    while ((uint)std::strlen(bufferOut) < (uint)numDigits + (signExist ? 2 : 1)) //-V2513
    {
        std::strcat(bufferOut, "0"); //-V2513
    }

    return bufferOut;
}



char* Int2String(int value, bool alwaysSign, int numMinFields, char buffer[20])
{
    char format[20] = "%";
    std::sprintf(&(format[1]), "0%d", numMinFields);
    std::strcat(format, "d"); //-V2513
    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        std::sprintf(buffer + 1, format, value);
    }
    else
    {
        std::sprintf(buffer, format, value);
    }
    return buffer;
}



bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    int length = static_cast<int>(std::strlen(str)); //-V2513
    if (length == 0)
    {
        return false;
    }
    *value = 0;
    int pow = 1;
    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    }
    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


char* Hex8toString(uint8 value, char buffer[3], bool upper)
{
    std::sprintf(value < 16 ? (buffer[0] = '0', buffer + 1) : (buffer), upper ? "%X" : "%x", value); //-V2528
    return buffer;
}


char* Hex16toString(uint16 value, char buffer[5], bool)
{
    std::sprintf(buffer, "%04X", value);
    return buffer;
}


char* Phase2String(float phase, bool, char bufferOut[20])
{
    char buffer[20];
    std::sprintf(bufferOut, "%s\xa8", Float2String(phase, false, 4, buffer));
    return bufferOut;
}


char* Float2Db(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    std::strcat(bufferOut, Float2String(value, false, numDigits, buffer)); //-V2513
    std::strcat(bufferOut, "Да"); //-V2513
    return bufferOut;
}



bool IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}



float MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if (val2 > retValue)
    {
        retValue = val2;
    }
    if (val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}



int8 CircleIncreaseInt8(int8 *val, int8 min, int8 max)
{
    (*val)++;
    if ((*val) > max)
    {
        (*val) = min;
    }
    return (*val);
}



int16 CircleIncreaseInt16(int16 *val, int16 min, int16 max)
{
    (*val)++;
    if ((*val) > max)
    {
        (*val) = min;
    }
    return (*val);
}



int CircleIncreaseInt(int *val, int min, int max)
{
    (*val)++;
    if ((*val) > max)
    {
        (*val) = min;
    }
    return (*val);
}



int8 CircleDecreaseInt8(int8 *val, int8 min, int8 max)
{
    (*val)--;
    if ((*val) < min)
    {
        (*val) = max;
    }
    return *val;
}



int16 CircleDecreaseInt16(int16 *val, int16 min, int16 max)
{
    (*val)--;
    if ((*val) < min)
    {
        (*val) = max;
    }
    return (*val);
}



int CircleDecreaseInt(int *val, int min, int max)
{
    (*val)--;
    if ((*val) < min)
    {
        (*val) = max;
    }
    return (*val);
}



float CircleAddFloat(float *val, float delta, float min, float max)
{
    *val += delta;
    if (*val > max)
    {
        *val = min;
    }
    return *val;
}



float CircleSubFloat(float *val, float delta, float min, float max)
{
    *val -= delta;
    if (*val < min)
    {
        *val = max;
    }
    return *val;
}



void AddLimitationFloat(float *val, float delta, float min, float max)
{
    float sum = *val + delta;
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



void SwapInt(int *value0, int *value1)
{
    int temp = *value0;
    *value0 = *value1;
    *value1 = temp;
}



void SortInt(int *value0, int *value1)
{
    if (*value1 < *value0)
    {
        SwapInt(value0, value1);
    }
}


void EmptyFuncVV(void)
{
}



void EmptyFuncVI(int)
{
}



void EmptyFuncVpV(void *)
{
}



bool EmptyFuncBV(void)
{
    return true;
}



void EmptyFuncpVII(void *, int, int)
{
}



void EmptyFuncVI16(int16)
{
}



void EmptyFuncVB(bool)
{
}



char* IntToStrCat(char *_buffer, int _value)
{
#define LENGTH_INTTOSTRCAT 10
    char buffer[LENGTH_INTTOSTRCAT];
    for (int i = 0; i < LENGTH_INTTOSTRCAT; i++)
    {
        buffer[i] = 0;
    }
    int pointer = LENGTH_INTTOSTRCAT - 1;

    while (_value > 0)
    {
        buffer[pointer] = (char)(_value % 10);
        _value /= 10;
        pointer--;
    }

    while (*_buffer)
    {
        _buffer++;
    }
    int i = 0;

    for (; i < LENGTH_INTTOSTRCAT; i++)
    {
        if (buffer[i] > 0)
        {
            break;
        }
    }

    for (; i < LENGTH_INTTOSTRCAT; i++)
    {
        *_buffer = 0x30 | buffer[i];
        _buffer++;
    }

    *_buffer = 0;

    return _buffer;
}



int BCD2Int(uint bcd)
{
    int pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}
