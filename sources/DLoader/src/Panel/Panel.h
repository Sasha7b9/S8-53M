#pragma once 
#include "common/Keyboard/Controls_.h"


struct Panel
{
    static bool   ProcessingCommandFromPIC(uint16 command);
    static uint16 NextData();
    static Key::E PressedButton();
};
