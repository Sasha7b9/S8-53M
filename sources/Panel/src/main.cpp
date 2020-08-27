#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"


int main(void)
{
    HAL::Init();

    Keyboard::Init();

    while (1)
    {
    }
}
