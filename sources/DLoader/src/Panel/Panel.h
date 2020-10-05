#pragma once 
#include "defines.h"
#include "Controls.h"


struct Panel
{
    static bool           ProcessingCommandFromPIC(uint16 command);
    static uint16         NextData();
    static PanelButton::E PressedButton();
};
