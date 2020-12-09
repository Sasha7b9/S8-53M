#include "defines.h"
#include "common/Utils/Math_c.h"
#include "Settings/Settings.h"


void HardwareErrorHandler(const char *, const char *, int)
{
    while(true)
    {
    }; 
};


uint Color::FromSettings(Color::E col)
{
    uint16 col16 = set.display.colors[col];

    uint b = (uint)((col16 & 0x1f) / 31.0F * 255.0F); //-V2533
    LIMIT_ABOVE(b, 255);
    uint g = (uint)(((col16 >> 5) & 0x3f) / 63.0F * 255.0F); //-V2533
    LIMIT_ABOVE(g, 255);
    uint r = (uint)(((col16 >> 11) & 0x1F) / 31.0F * 255.0F); //-V2533
    LIMIT_ABOVE(r, 255);

    return b | (g << 8) | (r << 16);
}
