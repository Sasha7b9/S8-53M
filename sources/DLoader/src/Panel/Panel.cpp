#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Keyboard/Controls_.h"
#include "Panel/Panel.h"


#define MAX_DATA            20

#define LED_CHAN0_ENABLE    129
#define LED_CHAN0_DISABLE   1
#define LED_CHAN1_ENABLE    130
#define LED_CHAN1_DISABLE   2
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4



static Key::E pressedButton = Key::None;



static Key::E ButtonIsPress(uint16 command);



Key::E Panel::PressedButton(void)
{
    return pressedButton;
}


uint16 Panel::NextData()
{
    return 0;
}


bool Panel::ProcessingCommandFromPIC(uint16 command)
{
    if(command)
    {
        Key::E prButton = ButtonIsPress(command);
        if(prButton)
        {
            pressedButton = prButton;
        }
        else
        {
            pressedButton = Key::None;
        }
    }
    return true;
}



static Key::E ButtonIsPress(uint16 command)
{
    Key::E button = Key::None;

    static uint timePrevPressButton = 0;

    if(command < (Key::Count | 0x80) && command > (Key::None | 0x80))
    {
        if(HAL_TIM2::TimeMS() - timePrevPressButton > 100)
        {
            button = (Key::E)(command & 0x7f); //-V2533
            timePrevPressButton = HAL_TIM2::TimeMS();
        }
    }

    return button;
}
