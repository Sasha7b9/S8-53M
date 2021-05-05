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
            static const int SIZE_BUFFER = 3;

            uint8 message[SIZE_BUFFER] = { 0xFF, (uint8)(event.key), (uint8)(event.action) };

            uint8 buffer_in[SIZE_BUFFER];

            HAL_SPI2::TransmitReceivce(message, buffer_in, SIZE_BUFFER);

            ProcessReceivedData(buffer_in, SIZE_BUFFER);
        }
    }


    if (HAL_SPI2::TimeAfterTransmit() > 10)
    {
        uint8 message[3] = { 0, 0, 0 };
        uint8 buffer_in[3] = { 0, 0, 0 };
        HAL_SPI2::TransmitReceivce(message, buffer_in, 3);
        ProcessReceivedData(buffer_in, 3);
    }
}


void Interface::ProcessReceivedData(uint8 *data, uint size)
{
    for (uint i = 0; i < size; i++)
    {
        ProcessByte(data[i]);
    }
}


void Interface::ProcessByte(uint8 byte)
{
    uint8 control = (uint8)(byte & 0x0F);

    if (control == TypeLED::Power)
    {
        pinPower.Reset();
    }

    if (control > 0 && control < 5)
    {
        static LED *leds[] = { nullptr, &led_Trig, &led_Set, &led_ChannelA, &led_ChannelB };

        LED *led = leds[control];

        (control == byte) ? led->Disable() : led->Enable();
    }
}
