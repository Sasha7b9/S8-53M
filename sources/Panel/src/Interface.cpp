#include "defines.h"
#include "LEDS.h"
#include "HAL.h"
#include "Interface.h"
#include "Keyboard.h"
#include "PowerSupply.h"


static const int SIZE_BUFFER = 3;

uint8 output[SIZE_BUFFER] = { 0xFF, 0, 0 };


void Interface::Update(KeyboardEvent &event)
{
    if (!PowerSupply::IsEnabled())
    {
        return;
    }

    bool needTransmit = false;

    output[1] = (uint8)event.key;
    output[2] = (uint8)event.action;

    if (event.key != Key::None)
    {
        needTransmit = true;
    }
    else if (HAL_SPI2::TimeAfterTransmit() > 10)
    {
        output[0] = 0x00;

        needTransmit = true;
    }

    if (needTransmit)
    {
        uint8 bufferIn[SIZE_BUFFER];

        HAL_SPI2::TransmitReceivce(output, bufferIn, SIZE_BUFFER);

        for (uint i = 0; i < SIZE_BUFFER; i++)
        {
            ProcessByte(bufferIn[i]);
        }
    }
}


void Interface::ProcessByte(uint8 byte)
{
    uint8 control = (uint8)(byte & 0x0F);

    if (control == TypeLED::Power)
    {
        PowerSupply::TurnOff();
    }
    else if (control > 0 && control < 5)
    {
        static LED *leds[] = { nullptr, &led_Trig, &led_Set, &led_ChannelA, &led_ChannelB };

        LED *led = leds[control];

        (control == byte) ? led->Disable() : led->Enable();
    }
}
