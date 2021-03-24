// 2021/03/24 10:56:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Containers/Values_.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>


String Float::ToString(bool always_sign, int num_digits)
{
    if (!IsValid())
    {
        return EmptyString();
    }

    String result;

    if (!always_sign)
    {
        if (value < 0.0f)
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

    int num_digits_in_int = GF::NumDigitsInIntPart(value);

    format[3] = (char)((num_digits - num_digits_in_int) + 0x30);
    if (num_digits == num_digits_in_int)
    {
        format[5] = '.';
    }

    std::sprintf(buffer, format, std::fabsf(value));

    result.Append(buffer);

    float val = (float)(std::atof(buffer)); //-V2508

    if (GF::NumDigitsInIntPart(val) != num_digits_in_int)
    {
        num_digits_in_int = GF::NumDigitsInIntPart(val);
        format[3] = (char)((num_digits - num_digits_in_int) + 0x30);

        if (num_digits == num_digits_in_int)
        {
            format[5] = '.';
        }

        std::sprintf(buffer, format, value);

        result.Append(buffer);
    }

    bool signExist = always_sign || value < 0.0f;

    while ((int)(result.Size()) < num_digits + (signExist ? 2 : 1))
    {
        result.Append("0");
    }

    return result;
}

String Voltage::ToString(bool always_sign)
{
    if (!IsValid())
    {
        return EmptyString();
    }

    char *suffix = nullptr;

    float voltage = *this;

    if (std::fabsf(voltage) + 0.5e-4F < 1e-3F)
    {
        suffix = LANG_RU ? "\x10ìêÂ" : "\x10uV";
        voltage *= 1e6F;
    }
    else if (std::fabsf(voltage) + 0.5e-4F < 1.0F)
    {
        suffix = LANG_RU ? "\x10ìÂ" : "\x10mV";
        voltage *= 1e3F;
    }
    else if (std::fabsf(voltage) + 0.5e-4F < 1000.0F)
    {
        suffix = LANG_RU ? "\x10Â" : "\x10V";
    }
    else
    {
        suffix = LANG_RU ? "\x10êÂ" : "\x10kV";
        voltage *= 1e-3F;
    }

    String result = GF::Float2String(voltage, always_sign, 4);
    result.Append(suffix);

    return result;
}


String Time::ToString(bool always_sign)
{
    if (!IsValid())
    {
        return EmptyString();
    }

    char *suffix = nullptr;

    float time = *this;

    if (std::fabsf(time) + 0.5e-10F < 1e-6F)
    {
        suffix = LANG_RU ? "íñ" : "ns";
        time *= 1e9F;
    }
    else if (std::fabsf(time) + 0.5e-7F < 1e-3F)
    {
        suffix = LANG_RU ? "ìêñ" : "us";
        time *= 1e6F;
    }
    else if (std::fabsf(time) + 0.5e-3F < 1.0F)
    {
        suffix = LANG_RU ? "ìñ" : "ms";
        time *= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "ñ" : "s";
    }

    return String("%s%s", GF::Float2String(time, always_sign, 4).c_str(), suffix);
}


String Frequency::ToString()
{
    if (!IsValid())
    {
        return EmptyString();
    }

    char *suffix = 0;

    float frequency = *this;

    if (frequency >= 1e6F)
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


String Phase::ToString()
{
    return String("%s\xa8", GF::Float2String(*this, false, 4).c_str());
}


String Int::ToString(bool always_sign, int num_min_fields)
{
    String result;

    if (always_sign && value >= 0)
    {
        result.Append('+');
    }

    char buffer[20];

    char format[20] = "%";
    std::sprintf(&(format[1]), "0%d", num_min_fields);
    std::strcat(format, "d");

    std::sprintf(buffer, format, value);

    result.Append(buffer);

    return result;
}
