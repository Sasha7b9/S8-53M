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
    bool turnOff = false;

    if (Keyboard::Buffer::IsEmpty())
    {
        turnOff = true;
    }

    while (!Keyboard::Buffer::IsEmpty())
    {
        KeyboardEvent event = Keyboard::Buffer::GetNextEvent();

        if (event.action == Action::Down || event.IsRotate())
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

    if (turnOff)
    {
        TurnOffLEDS();
    }
}


static void SendKeyboardEvent(KeyboardEvent event)
{
    uint8 message[3] = { 0xFF, static_cast<uint8>(event.key), static_cast<uint8>(event.action) };

    HAL_SPI2::Transmit(message, 3);
}


static uint timeOff = uint(-1);

static void TurnOnLEDS()
{
    led_ChannelA.Enable();
    led_ChannelB.Enable();
    led_Trig.Enable();
    led_Set.Enable();

    timeOff = HAL_TIM::TimeMS() + 100;
}


static void TurnOffLEDS()
{
    if (HAL_TIM::TimeMS() < timeOff)
    {
        return;
    }

    led_ChannelA.Disable();
    led_ChannelB.Disable();
    led_Trig.Disable();
    led_Set.Disable();
}
