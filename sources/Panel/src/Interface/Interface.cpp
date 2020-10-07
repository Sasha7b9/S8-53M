#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface.h"
#include "Keyboard/Keyboard.h"


// ѕосылает следующее событие клавиатуры в SPI
static void SendKeyboardEvent(KeyboardEvent event);


void Interface::Update()
{
    while (!Keyboard::Buffer::IsEmpty())
    {
        SendKeyboardEvent(Keyboard::Buffer::GetNextEvent());
    }
}

static void SendKeyboardEvent(KeyboardEvent event)
{
    uint8 message[3] = { 0xFF, static_cast<uint8>(event.key), static_cast<uint8>(event.action) };

    HAL_SPI2::Send(message, 3);
}
