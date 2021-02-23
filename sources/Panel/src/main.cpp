#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface.h"
#include "Keyboard/Keyboard.h"


int main(void)
{
    HAL::Init();

    Keyboard::Init();

    while (1)
    {
        Keyboard::Update();
//        Interface::Update();
        
        KeyboardEvent event;

        int numEvents = Keyboard::Buffer::NumEvents();

        if(numEvents == 0)
        {
        }
        else if (numEvents == 1)
        {
            event = Keyboard::Buffer::GetNextEvent();
            numEvents = 1;
        }
        else if(numEvents == 2)
        {
            event = Keyboard::Buffer::GetNextEvent();
            numEvents = 2;
        }
        else if(numEvents == 3)
        {
            event = Keyboard::Buffer::GetNextEvent();
            numEvents = numEvents;
        }
        else
        {
            event = Keyboard::Buffer::GetNextEvent();
            numEvents = numEvents;
        }

        while (!Keyboard::Buffer::IsEmpty())
        {
            event = Keyboard::Buffer::GetNextEvent();

            if (event.action == Action::Down)
            {
                event = event;
            }
        }
    }
}
