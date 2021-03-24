// 2021/03/24 10:56:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Containers/Values_.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstdio>
#include <cstring>



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
        suffix = LANG_RU ? "\x10���" : "\x10uV";
        voltage *= 1e6F;
    }
    else if (std::fabsf(voltage) + 0.5e-4F < 1.0F)
    {
        suffix = LANG_RU ? "\x10��" : "\x10mV";
        voltage *= 1e3F;
    }
    else if (std::fabsf(voltage) + 0.5e-4F < 1000.0F)
    {
        suffix = LANG_RU ? "\x10�" : "\x10V";
    }
    else
    {
        suffix = LANG_RU ? "\x10��" : "\x10kV";
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
        suffix = LANG_RU ? "��" : "ns";
        time *= 1e9F;
    }
    else if (std::fabsf(time) + 0.5e-7F < 1e-3F)
    {
        suffix = LANG_RU ? "���" : "us";
        time *= 1e6F;
    }
    else if (std::fabsf(time) + 0.5e-3F < 1.0F)
    {
        suffix = LANG_RU ? "��" : "ms";
        time *= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "�" : "s";
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
        suffix = LANG_RU ? "���" : "MHz";
        frequency /= 1e6F;
    }
    else if (frequency >= 1e3F)
    {
        suffix = LANG_RU ? "���" : "kHz";
        frequency /= 1e3F;
    }
    else
    {
        suffix = LANG_RU ? "��" : "Hz";
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
