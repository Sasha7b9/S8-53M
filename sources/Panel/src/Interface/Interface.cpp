#include "defines.h"
#include "Hardware/LEDS.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface.h"
#include "Keyboard/Keyboard.h"


// Посылает следующее событие клавиатуры в SPI
static void SendKeyboardEvent(KeyboardEvent event);

// Зажечь все светодиоды
static void TurnOnLEDS();

// Потушить все светодиоды
static void TurnOffLEDS();


void Interface::Update()
{
    while (!Keyboard::Buffer::IsEmpty())
    {
        KeyboardEvent event = Keyboard::Buffer::GetNextEvent();

        if (event.action == Action::Down)
        {
            TurnOffLEDS();
        }
        else
        {
            TurnOnLEDS();
        }


        if (pinON.Read() == 0)
        {
            if (event.key == Key::Power)
            {
                pinON.Set();
            }
        }
        else
        {
            SendKeyboardEvent(Keyboard::Buffer::GetNextEvent());
        }
    }
}


static void SendKeyboardEvent(KeyboardEvent event)
{
    uint8 message[3] = { 0xFF, static_cast<uint8>(event.key), static_cast<uint8>(event.action) };

    HAL_SPI2::Transmit(message, 3);
}


static void TurnOnLEDS()
{
    led_ChannelA.Enable();
    led_ChannelB.Enable();
    led_Trig.Enable();
    led_Set.Enable();
}


static void TurnOffLEDS()
{
    led_ChannelA.Disable();
    led_ChannelB.Disable();
    led_Trig.Disable();
    led_Set.Disable();
}
