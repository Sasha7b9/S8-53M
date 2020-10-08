#pragma once 
#include "defines.h"
#include "common/Keyboard/Controls.h"


struct Panel
{
    static bool   ProcessingCommandFromPIC(uint16 command);
    static uint16 NextData();
    static Key::E PressedButton();
};
