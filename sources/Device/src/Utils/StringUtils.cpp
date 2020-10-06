#include "defines.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdlib>


char SU::ToUpper(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if (s == 0x40)        // ¸
    {
        return '\xc5';
    }
    else if (s >= 0x60 && s <= 0x7a)
    {
        return static_cast<char>(s - 0x20);
    }
    else if (s >= 0xf0)
    {
        return static_cast<char>(s - 0x20);
    }

    return symbol;
}


char SU::ToLower(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if (s >= 0x41 && s <= 0x5a)
    {
        return static_cast<char>(s + 0x20);
    }
    else if (s >= 0xc0 && s < 0xE0)
    {
        return static_cast<char>(s + 0x20);
    }

    return symbol;
}


char *SU::DoubleToString(double value)
{
    return "";
}


bool SU::StringToDouble(double *value, const char *str)
{
    static const int SIZE_BUFFER = 100;
    char buffer[SIZE_BUFFER];

    strcpy_s(buffer, SIZE_BUFFER - 1, str);

    char *p = buffer;

    while (*p)
    {
        if (*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    char *end = nullptr;

    *value = std::strtod(buffer, &end);

    return (end != buffer);
}
