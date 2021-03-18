#include "defines.h"
#include "common/Utils/String_.h"
#include "common/Display/Colors_.h"


void Color::SetAsCurrent() const
{
    if (index != Count)
    {
        current = index;
    }
}
