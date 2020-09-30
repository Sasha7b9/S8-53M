#pragma once


#include "Strings.h"


struct MapElement
{
    char *key;
    uint8 value;
};


uint8 GetValueFromMap(const MapElement *map, Word *key);      // ≈сли значение не найдено, возвращеетс€ 255;
