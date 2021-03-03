#pragma once 
#include "common/Panel/Controls_.h"


struct Panel
{
    static bool   ProcessingCommandFromPIC(uint16 command);
    static uint16 NextData();
    static Key::E PressedButton();
    
    static void CallbackOnReceiveSPI5(uint8 *data, int size);
};
