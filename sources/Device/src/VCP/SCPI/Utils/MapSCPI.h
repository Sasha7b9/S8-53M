#pragma once
#include "VCP/SCPI/Utils/StringsSCPI.h"


struct MapElement
{
    char *key;
    uint8 value;
};


uint8 GetValueFromMap(const MapElement *map, Word *key);      // ≈сли значение не найдено, возвращеетс€ 255;
