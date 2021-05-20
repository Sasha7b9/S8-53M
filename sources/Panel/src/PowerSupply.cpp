// 2021/05/05 15:03:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "HAL.h"
#include "Keyboard.h"
#include "PowerSupply.h"


uint PowerSupply::timeEnabled = (uint)-1;


bool PowerSupply::IsEnabled()
{
    return timeEnabled != (uint)-1;
}


void PowerSupply::AttemptToTurnOn(KeyboardEvent &event)
{
    if (event.key == Key::Power)
    {
        if (timeEnabled == (uint)-1)
        {
            pinPower.Set();

            timeEnabled = TIME_MS;
        }
    }
}


void PowerSupply::TurnOff()
{
    pinPower.Reset();

    timeEnabled = (uint)-1;
}
