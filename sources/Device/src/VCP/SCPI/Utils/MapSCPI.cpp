#include "defines.h"
#include "VCP/SCPI/Utils/MapSCPI.h"
#include "VCP/SCPI/Utils/StringsSCPI.h"


uint8 GetValueFromMap(const MapElement *map, Word *eKey) //-V2506
{
    int numKey = 0;
    char *key = map[numKey].key; //-V2563
    while (key != 0)
    {
        if (WordEqualZeroString(eKey, key))
        {
            return map[numKey].value; //-V2563
        }
        numKey++;
        key = map[numKey].key; //-V2563
    }
    return 255;
}
