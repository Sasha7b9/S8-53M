#pragma once
#include "common/Utils/Containers/String_.h"


struct Float;


namespace GF
{
    String Hex8toString(uint8 value);

    // ѕреобразует децибелы в текстовую строку.
    String Float2Db(float value, int num_digits);

    String LogArrayUint8(uint8 *array, uint size);

    int NumDigitsInIntPart(float value);
}
