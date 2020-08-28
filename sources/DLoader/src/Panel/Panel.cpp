#include "Panel.h"
#include "Controls.h"
#include "Utils/Math.h"
#include "Utils/GlobalFunctions.h"
#include "Settings/Settings.h"
#include "common/Hardware/HAL/HAL.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include <cstdio>
#include <cstring>


#define MAX_DATA            20

#define LED_CHAN0_ENABLE    129
#define LED_CHAN0_DISABLE   1
#define LED_CHAN1_ENABLE    130
#define LED_CHAN1_DISABLE   2
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4



static PanelButton pressedButton = B_Empty;



static PanelButton ButtonIsPress(uint16 command);



PanelButton Panel_PressedButton(void)
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
        PanelButton prButton = ButtonIsPress(command);
        if(prButton)
        {
            pressedButton = prButton;
        }
        else
        {
            pressedButton = B_Empty;
        }
    }
    return true;
}



static PanelButton ButtonIsPress(uint16 command)
{
    PanelButton button = B_Empty;

    static uint timePrevPressButton = 0;

    if(command < (B_NumButtons | 0x80) && command > (B_Empty | 0x80))
    {
        if(HAL_TIM2::TimeMS() - timePrevPressButton > 100)
        {
            button = (PanelButton)(command & 0x7f);
            timePrevPressButton = HAL_TIM2::TimeMS();
        }
    }

    return button;
}
