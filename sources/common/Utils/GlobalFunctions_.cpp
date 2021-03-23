#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace GF
{
    template int8 CircleIncrease(int8 *val, int8 min, int8 max);
    template int16 CircleIncrease(int16 *val, int16 min, int16 max);
    template int CircleIncrease(int *val, int min, int max);

    template int8 CircleDecrease(int8 *val, int8 min, int8 max);
    template int16 CircleDecrease(int16 *val, int16 min, int16 max);
    template int CircleDecrease(int *val, int min, int max);
}


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

    float val = static_cast<float>(std::atof(buffer)); //-V2508

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
    std::strcat(format, "d");

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
    int length = (int)(std::strlen(str));
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

String GF::Voltage2String(const Float &volt, bool always_sign)
{
    if(!volt.IsValid())
    {
        return EmptyString();
    }

    char *suffix = nullptr;

    float voltage = volt;
    
    if(std::fabsf(voltage) + 0.5e-4F < 1e-3F)
    {
        suffix = LANG_RU ? "\x10ìêÂ" : "\x10uV";
        voltage *= 1e6F;
    }
    else if(std::fabsf(voltage) + 0.5e-4F < 1.0F)
    {
        suffix = LANG_RU ? "\x10ìÂ" : "\x10mV" ;
        voltage *= 1e3F;
    }
    else if(std::fabsf(voltage) + 0.5e-4F < 1000.0F)
    {
        suffix = LANG_RU ? "\x10Â" : "\x10V";
    }
    else
    {
        suffix = LANG_RU ? "\x10êÂ" : "\x10kV";
        voltage *= 1e-3F;
    }

    String result = Float2String(voltage, always_sign, 4);
    result.Append(suffix);

    return result;
}

String GF::Time2String(const Float &tim, bool always_sign)
{
    if(!tim.IsValid())
    {
        return EmptyString();
    }
    
    char *suffix = nullptr;

    float time = tim;
    
    if(std::fabsf(time) + 0.5e-10F < 1e-6F)
    {
        suffix = LANG_RU ? "íñ" : "ns";
        time *= 1e9F;
    }
    else if(std::fabsf(time) + 0.5e-7F < 1e-3F)
    {
        suffix = LANG_RU ? "ìêñ" : "us";
        time *= 1e6F;
    }
    else if(std::fabsf(time) + 0.5e-3F < 1.0F)
    {
        suffix = LANG_RU ? "ìñ" : "ms";
        time *= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "ñ" : "s";
    }

    return String("%s%s", Float2String(time, always_sign, 4).c_str(), suffix);
}

String GF::Phase2String(float phase, bool)
{
    return String("%s\xa8", GF::Float2String(phase, false, 4).c_str());
}

String GF::Freq2String(const Float &freq, bool)
{
    if(!freq.IsValid())
    {
        return EmptyString();
    }

    char *suffix = 0;

    float frequency = freq;

    if(frequency >= 1e6F)
    {
        suffix = LANG_RU ? "ÌÃö" : "MHz";
        frequency /= 1e6F;
    }
    else if (frequency >= 1e3F)
    {
        suffix = LANG_RU ? "êÃö" : "kHz";
        frequency /= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "Ãö" : "Hz";
    }

    return String("%s%s", GF::Float2String(frequency, false, 4).c_str(), suffix);
}

String GF::Float2Db(float value, int num_digits)
{
    return String("%säÁ", GF::Float2String(value, false, num_digits).c_str());
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


String GF::LogArrayUint8(uint8 *array, uint size)
{
    String result;

    for (uint i = 0; i < size; i++)
    {
        result.Append(GF::Int2String(array[i], false, 1).c_str());
        
        if (i != size - 1)
        {
            result.Append(' ');
        }
    }

    return result;
}
