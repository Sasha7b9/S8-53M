#include "defines.h"
#include "common/Display/Colors_.h"


Color Color::Trig()
{
    if (TrigSource::IsA() || TrigSource::IsB())
    {
        return Channel((Channel::E)TrigSource::Get()).GetColor();
    }

    return Color::FILL;
}
