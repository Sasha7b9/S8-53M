#include "defines.h"
#include "common/Display/Colors_.h"
#include "Settings/Settings.h"


Color Color::Trig()
{
    TrigSource::E trigChan = TRIG_SOURCE;
    if (trigChan == TrigSource::A || trigChan == TrigSource::B)
    {
        return Color::Channel((Channel::E)trigChan);
    }
    return Color::FILL;
}
