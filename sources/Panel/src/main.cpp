#include "defines.h"
#include "HAL.h"
#include "Interface.h"
#include "Keyboard.h"


int main()
{
    HAL::Init();

    Keyboard::Init();

    while (1)
    {
        Keyboard::Update();

        Interface::Update();
    }
}
