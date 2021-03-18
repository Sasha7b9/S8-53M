#include "defines.h"
#include "common/Display/Colors_.h"
#include "Settings/Settings.h"


Color Color::Trig()
{
    TrigSource::E trigChan = TRIG_SOURCE;

    if (trigChan == TrigSource::A_ || trigChan == TrigSource::B_)
    {
        return Channel((Channel::E)trigChan).GetColor();
    }

    return Color::FILL;
}
