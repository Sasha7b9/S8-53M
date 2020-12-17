#include "defines.h"
#include "common/Display/Colors_c.h"
#include "Settings/Settings.h"


Color Color::Trig()
{
    TrigSource::E trigChan = TRIG_SOURCE;
    if (trigChan == TrigSource::A || trigChan == TrigSource::B)
    {
        return Color::Channel((Channel::E)trigChan); //-V2533
    }
    return Color::FILL;
}
