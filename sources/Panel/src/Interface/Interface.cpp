#include "defines.h"
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

static void SendKeyboardEvent(KeyboardEvent )
{

}
