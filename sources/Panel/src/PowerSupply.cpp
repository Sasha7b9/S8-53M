// 2021/05/05 15:03:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "HAL.h"
#include "Keyboard.h"
#include "PowerSupply.h"


bool PowerSupply::IsEnabled()
{
    return pinPower.Read() == 1;
}


void PowerSupply::Update()
{
    if (!Keyboard::Buffer::IsEmpty())
    {
        KeyboardEvent event = Keyboard::Buffer::GetNextEvent();

        if (event.key == Key::Power)
        {
            pinPower.Set();
        }
    }
}
