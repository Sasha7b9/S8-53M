// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 
#include "common/Panel/Controls_.h"


struct Panel
{
    static uint16 NextData();
    static Key::E PressedButton();
    
    static void CallbackOnReceiveSPI5(uint8 *data, uint size);
};
