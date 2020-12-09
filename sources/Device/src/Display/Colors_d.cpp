#include "defines.h"
#include "common/Display/Colors_c.h"
#include "Settings/Settings.h"


Color::E Color::Trig() //-V2506
{
    TrigSource::E trigChan = TRIG_SOURCE;
    if (trigChan == TrigSource::A || trigChan == TrigSource::B)
    {
        return Color::Channel((Channel::E)trigChan);
    }
    return Color::FILL;
}
