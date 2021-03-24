// 2021/03/24 10:56:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Containers/Values_.h"
#include "Settings/Settings.h"
#include <cmath>



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
