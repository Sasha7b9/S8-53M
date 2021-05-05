#include "defines.h"
#include "HAL.h"
#include "Interface.h"
#include "Keyboard.h"
#include "PowerSupply.h"


int main()
{
    HAL::Init();

    Keyboard::Init();

    while (!PowerSupply::IsEnabled())
    {
        Keyboard::Update();

        PowerSupply::Update();
    }


    while (1)
    {
        Keyboard::Update();

        Interface::Update();
    }
}
