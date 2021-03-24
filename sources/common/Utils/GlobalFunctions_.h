#pragma once
#include "common/Utils/Containers/String_.h"


struct Float;


namespace GF
{
    String LogArrayUint8(uint8 *array, uint size);

    int NumDigitsInIntPart(float value);
}
