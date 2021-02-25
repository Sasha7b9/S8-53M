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

        Interface::Update();
    }
}
