// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "VCP/SCPI/Utils/MapSCPI.h"
#include "VCP/SCPI/Utils/StringsSCPI.h"


uint8 GetValueFromMap(const MapElement *map, Word *eKey)
{
    int numKey = 0;
    char *key = map[numKey].key;
    while (key != 0)
    {
        if (WordEqualZeroString(eKey, key))
        {
            return map[numKey].value;
        }
        numKey++;
        key = map[numKey].key;
    }
    return 255;
}
