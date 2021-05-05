#include "defines.h"
#include "LEDS.h"
#include "HAL.h"
#include "Interface.h"
#include "Keyboard.h"


void Interface::Update()
{
    KeyboardEvent event = Keyboard::Buffer::GetNextEvent();

    static const int SIZE_BUFFER = 3;

    uint8 buffer_out[SIZE_BUFFER] = { 0xFF, (uint8)(event.key), (uint8)(event.action) };

    bool need_transmit = false;

    if (event.key != Key::None && event.key != Key::Power)
    {
        need_transmit = true;
    }
    else if (HAL_SPI2::TimeAfterTransmit() > 10)
    {
        buffer_out[0] = 0x00;

        need_transmit = true;
    }

    if (need_transmit)
    {
        uint8 buffer_in[SIZE_BUFFER];

        HAL_SPI2::TransmitReceivce(buffer_out, buffer_in, SIZE_BUFFER);

        for (uint i = 0; i < SIZE_BUFFER; i++)
        {
            ProcessByte(buffer_in[i]);
        }
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
