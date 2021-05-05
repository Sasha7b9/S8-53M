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

}
