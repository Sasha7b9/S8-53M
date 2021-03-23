#include "defines.h"
#include "common/Utils/StringUtils_.h"
#include <cstring>
#include <cstdlib>


char SU::ToUpper(char symbol)
{
    uint8 s = (uint8)(symbol);

    if (s == 0x40)        // ¸
    {
        return '\xc5';
    }
    else if (s >= 0x60 && s <= 0x7a)
    {
        return (char)(s - 0x20);
    }
    else if (s >= 0xf0)
    {
        return (char)(s - 0x20);
    }

    return symbol;
}


char SU::ToLower(char symbol)
{
    uint8 s = (uint8)(symbol);

    if (s >= 0x41 && s <= 0x5a)
    {
        return (char)(s + 0x20);
    }
    else if (s >= 0xc0 && s < 0xE0)
    {
        return (char)(s + 0x20);
    }

    return symbol;
}


char *SU::DoubleToString(double)
{
    return "";
}


bool SU::StringToDouble(double *, pchar)
{
    return false;
}
