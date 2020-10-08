#include "defines.h"
#include "common/Display/Colors.h"
#include "Settings/Settings.h"


Color::E ColorTrig()
{
    TrigSource::E trigChan = TRIG_SOURCE;
    if (trigChan == TrigSource::A || trigChan == TrigSource::B)
    {
        return ColorChannel((Channel::E)trigChan);
    }
    return COLOR_FILL;
}
