#include "defines.h"
#include "LEDS.h"
#include "HAL.h"
#include "Interface.h"
#include "Keyboard.h"


void Interface::Update()
{
    while (!Keyboard::Buffer::IsEmpty())
    {
        KeyboardEvent event = Keyboard::Buffer::GetNextEvent();

        if (pinPower.Read() == 0)
        {
            if (event.key == Key::Power)
            {
                if (event.IsUp() || event.IsLong())
                {
                    pinPower.Set();
                }
            }
        }
        else
        {
            uint8 message[3] = { 0xFF, (uint8)(event.key), (uint8)(event.action) };

            HAL_SPI2::Transmit(message, 3);
        }
    }
}
