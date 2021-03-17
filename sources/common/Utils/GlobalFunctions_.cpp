#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace GF
{
    template int8 CircleIncrease(int8 *val, int8 min, int8 max);
    template int16 CircleIncrease(int16 *val, int16 min, int16 max);
    template int CircleIncrease(int *val, int min, int max);

    template int8 CircleDecrease(int8 *val, int8 min, int8 max);
    template int16 CircleDecrease(int16 *val, int16 min, int16 max);
    template int CircleDecrease(int *val, int min, int max);
}


char *GF::FloatFract2String(float value, bool alwaysSign, char bufferOut[20])
{
    return GF::Float2String(value, alwaysSign, 4, bufferOut);
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

char* GF::Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char *pBuffer = bufferOut;

    if(value == ERROR_VALUE_FLOAT) //-V550 //-V2550
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE); //-V2513
        return bufferOut;
    }

    if(!alwaysSign)
    {
        if(value < 0)
        {
            *pBuffer = '-';
            pBuffer++;
        }
    }
    else
    {
        *pBuffer = value < 0 ? '-' : '+';
        pBuffer++;
    }

    char format[] = "%4.2f\0\0";

    format[1] = (char)numDigits + 0x30;

    int numDigitsInInt = NumDigitsInIntPart(value);

    format[3] = static_cast<char>((numDigits - numDigitsInInt) + 0x30);
    if(numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }
    
    std::snprintf(pBuffer, 19, format, std::fabsf(value));

    float val = static_cast<float>(std::atof(pBuffer)); //-V2508

    if (NumDigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = NumDigitsInIntPart(val);
        format[3] = static_cast<char>((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        std::sprintf(pBuffer, format, value);
    }

    bool signExist = alwaysSign || value < 0;
    while(static_cast<int>(std::strlen(bufferOut)) < numDigits + (signExist ? 2 : 1)) //-V2513
    {
        std::strcat(bufferOut, "0"); //-V2513
    }

    return bufferOut;
}

String GF::Int2String(int value, bool alwaysSign, int numMinFields)
{
    String result;

    if (alwaysSign && value >= 0)
    {
        result.Append('+');
    }

    char buffer[20];

    char format[20] = "%";
    std::sprintf(&(format[1]), "0%d", numMinFields);
    std::strcat(format, "d"); //-V2513

    std::sprintf(buffer, format, value);

    result.Append(buffer);

    return result;
}

bool GF::String2Int(char *str, int *value)  
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
    for(int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if(val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    }
    if(sign == -1)
    {
        *value *= -1;
    }
    return true;
}

char *GF::Bin2String(uint8 value, char buffer[9])
{
    for(int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    return buffer;
}

char *GF::Bin2String16(uint16 value, char valBuffer[19])
{
    char buffer[9];
    std::strcpy(valBuffer, Bin2String((uint8)(value >> 8), buffer)); //-V2513
    valBuffer[8] = ' ';
    std::strcpy(valBuffer + 9, Bin2String((uint8)value, buffer)); //-V2513
    return valBuffer;
}

char *GF::Hex8toString(uint8 value, char buffer[3])
{
    std::sprintf(value < 16 ? (buffer[0] = '0', buffer + 1) :  (buffer), "%x", value); //-V2528
    return buffer;
}

char *GF::Voltage2String(float voltage, bool alwaysSign, char buffer[20])
{
    buffer[0] = 0;
    char *suffix;
    if(voltage == ERROR_VALUE_FLOAT) //-V2550 //-V550
    {
        std::strcat(buffer, ERROR_STRING_VALUE); //-V2513
        return buffer;
    }
    else if(std::fabsf(voltage) + 0.5e-4F < 1e-3F)
    {
        suffix = LANG_RU ? "\x10���" : "\x10uV";
        voltage *= 1e6F;
    }
    else if(std::fabsf(voltage) + 0.5e-4F < 1.0F)
    {
        suffix = LANG_RU ? "\x10��" : "\x10mV" ;
        voltage *= 1e3F;
    }
    else if(std::fabsf(voltage) + 0.5e-4F < 1000.0F)
    {
        suffix = LANG_RU ? "\x10�" : "\x10V";
    }
    else
    {
        suffix = LANG_RU ? "\x10��" : "\x10kV";
        voltage *= 1e-3F;
    }

    char bufferOut[20];

    GF::Float2String(voltage, alwaysSign, 4, bufferOut);
    std::strcat(buffer, bufferOut); //-V2513
    std::strcat(buffer, suffix); //-V2513
    return buffer;
}

char* GF::Time2String(float time, bool alwaysSign, char buffer[20])
{
    buffer[0] = 0;
    char *suffix = 0;
    if(time == ERROR_VALUE_FLOAT) //-V2550 //-V550
    {
        std::strcat(buffer, ERROR_STRING_VALUE); //-V2513
        return buffer;
    }
    else if(std::fabsf(time) + 0.5e-10F < 1e-6F)
    {
        suffix = LANG_RU ? "��" : "ns";
        time *= 1e9F;
    }
    else if(std::fabsf(time) + 0.5e-7F < 1e-3F)
    {
        suffix = LANG_RU ? "���" : "us";
        time *= 1e6F;
    }
    else if(std::fabsf(time) + 0.5e-3F < 1.0F)
    {
        suffix = LANG_RU ? "��" : "ms";
        time *= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "�" : "s";
    }

    char bufferOut[20];
    std::strcat(buffer, GF::Float2String(time, alwaysSign, 4, bufferOut)); //-V2513
    std::strcat(buffer, suffix); //-V2513
    return buffer;
}

char *GF::Phase2String(float phase, bool, char bufferOut[20])
{
    char buffer[20];
    std::sprintf(bufferOut, "%s\xa8", GF::Float2String(phase, false, 4, buffer));
    return bufferOut;
}

char *GF::Freq2String(float freq, bool, char bufferOut[20])
{
    bufferOut[0] = 0;
    char *suffix = 0;
    if(freq == ERROR_VALUE_FLOAT) //-V2550 //-V550
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE); //-V2513
        return bufferOut;
    }
    if(freq >= 1e6F)
    {
        suffix = LANG_RU ? "���" : "MHz";
        freq /= 1e6F;
    }
    else if (freq >= 1e3F)
    {
        suffix = LANG_RU ? "���" : "kHz";
        freq /= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "��" : "Hz";
    }
    char buffer[20];
    std::strcat(bufferOut, GF::Float2String(freq, false, 4, buffer)); //-V2513
    std::strcat(bufferOut, suffix); //-V2513
    return bufferOut;
}

char* GF::Float2Db(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    std::strcat(bufferOut, GF::Float2String(value, false, numDigits, buffer)); //-V2513
    std::strcat(bufferOut, "��"); //-V2513
    return bufferOut;
}

bool GF::IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}

float GF::MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if(val2 > retValue)
    {
        retValue = val2;
    }
    if(val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}


template<class T>
T GF::CircleIncrease(T *val, T min, T max)
{
    (*val)++;

    if ((*val) > max)
    {
        (*val = min);
    }

    return (*val);
}


template<class T>
T GF::CircleDecrease(T *val, T min, T max)
{
    (*val)--;

    if((*val) < min)
    {
        (*val) = max;
    }

    return *val;
}


float GF::CircleAddFloat(float *val, float delta, float min, float max)
{
    *val += delta;
    if(*val > max)
    {
        *val = min;
    }
    return *val;
}


float GF::CircleSubFloat(float *val, float delta, float min, float max)
{
    *val -= delta;
    if(*val < min)
    {
        *val = max;
    }
    return *val;
}


void GF::AddLimitationFloat(float *val, float delta, float min, float max)
{
    float sum = *val + delta;
    if(sum < min)
    {
        *val = min;
    }
    else if(sum > max)
    {
        *val = max;
    }
    else
    {
        *val = sum;
    }
}


void GF::SwapInt(int *value0, int *value1)
{
    int temp = *value0;
    *value0 = *value1;
    *value1 = temp;
}


void GF::SortInt(int *value0, int *value1)
{
    if(*value1 < *value0)
    {
        SwapInt(value0,  value1);
    }
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


String GF::LogArrayUint8(uint8 *array, int size)
{
    String result;

    for (int i = 0; i < size; i++)
    {
        result.Append(GF::Int2String(array[i], false, 1).c_str());
        
        if (i != size - 1)
        {
            result.Append(' ');
        }
    }

    return result;
}
