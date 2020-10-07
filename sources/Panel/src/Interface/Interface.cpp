#include "defines.h"
#include "Interface/Interface.h"
#include "Keyboard/Keyboard.h"


// �������� ��������� ������� ���������� � SPI
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
