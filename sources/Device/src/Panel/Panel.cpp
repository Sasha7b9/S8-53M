#include "defines.h"
#include "Panel/Panel.h"
#include "Panel/PanelFunctions.cpp"
#include <cstdio>
#include <cstring>




#define MAX_DATA            20

#define LED_CHAN0_ENABLE    129U
#define LED_CHAN0_DISABLE   1U
#define LED_CHAN1_ENABLE    130U
#define LED_CHAN1_DISABLE   2U
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4

static Key::E pressedKey = Key::None;
volatile static Key::E pressedButton = Key::None;         // Это используется для отслеживания нажатой кнопки при отключенной панели
static uint16 dataTransmitted[MAX_DATA] = {0x00};
static uint16 numDataForTransmitted = 0;
static uint timePrevPressButton = 0;
static uint timePrevReleaseButton = 0;


static void(*funcOnKeyDown[Key::Count])(void)    =
{    
    0,
    EmptyFuncVV,    // Key::F1
    EmptyFuncVV,    // Key::F2
    EmptyFuncVV,    // Key::F3
    EmptyFuncVV,    // Key::F4
    EmptyFuncVV,    // Key::F5
    EmptyFuncVV,    // Key::ChannelA
    EmptyFuncVV,    // Key::ChannelB
    EmptyFuncVV,    // Key::Time
    EmptyFuncVV,    // B_Sinchro
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    EmptyFuncVV,    // Key::Help
    StartDown,      // Key::Start
    EmptyFuncVV,    // Key::Memory                                         
    EmptyFuncVV,    // Key::Service
    EmptyFuncVV,    // Key::Menu
    PowerDown       // B_Power
};

static void (*funcOnKeyUp[Key::Count])(void)    =
{
    0,
    EmptyFuncVV,    // Key::F1
    EmptyFuncVV,    // Key::F2
    EmptyFuncVV,    // Key::F3
    EmptyFuncVV,    // Key::F4
    EmptyFuncVV,    // Key::F5
    EmptyFuncVV,    // Key::ChannelA
    EmptyFuncVV,    // Key::ChannelB
    EmptyFuncVV,    // Key::Time
    EmptyFuncVV,    // B_Sinchro
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    EmptyFuncVV,    // Key::Help
    EmptyFuncVV,    // Key::Start
    EmptyFuncVV,    // Key::Memory
    EmptyFuncVV,    // Key::Service
    EmptyFuncVV,    // Key::Menu
    EmptyFuncVV     // B_Power
};

static void (*funcOnLongPressure[Key::Count])(void)    =
{
    0,
    F1Long,         // Key::F1
    F2Long,         // Key::F2
    F3Long,         // Key::F3
    F4Long,         // Key::F4
    F5Long,         // Key::F5
    Channel0Long,   // Key::ChannelA
    Channel1Long,   // Key::ChannelB
    TimeLong,       // Key::Time
    TrigLong,       // B_Sinchro
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    HelpLong,       // Key::Help
    EmptyFuncVV,    // Key::Start
    EmptyFuncVV,    // Key::Memory
    EmptyFuncVV,    // Key::Service
    MenuLong,       // Key::Menu
    EmptyFuncVV     // B_Power
};

static void (*funculatorLeft[Key::Count])(void)    =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RangeLeftA,     // R_Range0
    RangeLeftB,     // R_Range1
    RShiftLeftA,    // R_RShift0
    RShiftLeftB,    // R_RShift1
    TBaseLeft,      // R_TBase
    TShiftLeft,     // R_TShift
    TrigLevLeft,    // R_TrigLev
    SetLeft         // Key::Setting
};
static void (*funculatorRight[Key::Count])(void) =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RangeRightA,    // R_Range0
    RangeRightB,    // R_Range1
    RShiftRightA,   // R_RShift0
    RShiftRightB,   // R_RShift1
    TBaseRight,     // R_TBase
    TShiftRight,    // R_TShift
    TrigLevRight,   // R_TrigLev
    SetRight        // Key::Setting
};



Key::E ButtonIsRelease(uint16 command)
{
    Key::E key = Key::FromCode(command);
    Action::E action = Action::FromCode(command);

    if (Key::IsButton(key) && (action == Action::Up))
    {
        return key;
    }

    return Key::None;
}

Key::E ButtonIsPress(uint16 command)
{
    Key::E key = Key::FromCode(command);
    Action::E action = Action::FromCode(command);

    if (Key::IsButton(key) && (action == Action::Down))
    {
        return key;
    }

    return Key::None;
}

Key::E RegulatorLeft(uint16 command)
{
    Key::E key = Key::FromCode(command);
    Action::E action = Action::FromCode(command);

    if (Key::IsGovernor(key) && (action == Action::RotateLeft))
    {
        return key;
    }

    return Key::None;
}

Key::E RegulatorRight(uint16 command)
{
    Key::E key = Key::FromCode(command);
    Action::E action = Action::FromCode(command);

    if (Key::IsGovernor(key) && (action == Action::RotateRight))
    {
        return key;
    }

    return Key::None;
}

void OnTimerPressedKey()
{
    if(pressedKey != Key::None)
    {
        void (*func)(void) = funcOnLongPressure[pressedKey];
        Menu::ReleaseButton(pressedKey);
        if(func != 0)
        {
            func();
        }
        pressedKey = Key::None;
    }
    Timer::Disable(TypeTimer::PressKey);
}

bool Panel::ProcessingCommandFromPIC(uint16 command)
{
    static int allRecData = 0;

    Key::E releaseButton = ButtonIsRelease(command);
    Key::E pressButton = ButtonIsPress(command);
    Key::E regLeft = RegulatorLeft(command);
    Key::E regRight = RegulatorRight(command);

    if (pressButton != Key::None)
    {
        pressedButton = pressButton;
    }

    if(PANEL_IS_RUNNING == 0)
    {
        return true;
    }

    if(command != 0)
    {
        allRecData++;
    }
    else
    {
        allRecData++;
        allRecData--;
    }

    if(releaseButton != Key::None)
    {
        Menu::ReleaseButton(releaseButton);
        funcOnKeyUp[releaseButton]();
        if(pressedKey != Key::None)
        {
            Menu::ShortPressureButton(releaseButton);
            pressedKey = Key::None;
        }
    }
    else if(pressButton != Key::None)
    {
        funcOnKeyDown[pressButton]();
        Menu::PressButton(pressButton);
        pressedKey = pressButton;
        Timer::Enable(TypeTimer::PressKey, 500, OnTimerPressedKey);
    }
    else if(regLeft != Key::None)
    {
        funculatorLeft[regLeft](); //-V557
    }
    else if(regRight != Key::None)
    {
        funculatorRight[regRight]();
    }

    if ((command > Key::Setting && command < (Key::None + 1 + 128)) || (command > (Key::Setting + 128)))
    {
        if(Settings::DebugModeEnable())
        {
            static int errRecData = 0;
            errRecData++;
            float percent = (float)errRecData / allRecData * 100.0F;
            char buffer[100];
            buffer[0] = 0;
            std::sprintf(buffer, "%5.3f", percent);
            std::strcat(buffer, "%");
            LOG_ERROR("Ошибок SPI - %s %d/%d, command = %d", buffer, errRecData, allRecData, (int)command);
        }
        return false;
    }
    return true;
}

void Panel::EnableLEDChannel0(bool enable)
{
    Panel::TransmitData(enable ? LED_CHAN0_ENABLE : LED_CHAN0_DISABLE);
}

void Panel::EnableLEDChannel1(bool enable)
{
    Panel::TransmitData(enable ? LED_CHAN1_ENABLE : LED_CHAN1_DISABLE);
}

void Panel::EnableLEDTrig(bool enable)
{
    static uint timeEnable = 0;
    static bool first = true;
    static bool fired = false;
    if(first)
    {
        Panel::TransmitData(LED_TRIG_DISABLE);
        Display::EnableTrigLabel(false);
        timeEnable = gTimerMS;
        first = false;
    }

    if(enable)
    {
        timeEnable = gTimerMS;
    }

    if(enable != fired)
    {
        if(enable)
        {
            Panel::TransmitData(LED_TRIG_ENABLE);
            Display::EnableTrigLabel(true);
            fired = true;
        }
        else if(gTimerMS - timeEnable > 100)
        {
            Panel::TransmitData(LED_TRIG_DISABLE);
            Display::EnableTrigLabel(false);
            fired = false;
        }
    }
}

void Panel::TransmitData(uint16 data)
{
    if(numDataForTransmitted >= MAX_DATA)
    {
        LOG_WRITE("Не могу послать в панель - мало места");
    }
    else
    {
        dataTransmitted[numDataForTransmitted] = data;
        numDataForTransmitted++;
    }
}

uint16 Panel::NextData()
{
    if (numDataForTransmitted > 0)
    {
        numDataForTransmitted--;
        return dataTransmitted[numDataForTransmitted];
    }
    return 0;
}

void Panel::Disable()
{
    PANEL_IS_RUNNING = 0;
}

void Panel::Enable()
{
    PANEL_IS_RUNNING = 1;
}


void Panel::Init()
{


    // Лампочка УСТАНОВКА  pinLED

    Panel::EnableLEDRegSet(false);
}

void Panel::EnableLEDRegSet(bool enable)
{
    pinLED.Write(enable ? 1 : 0);
}

Key::E Panel::WaitPressingButton()
{
    pressedButton = Key::None;
    while (pressedButton == Key::None) {};
    return pressedButton;
}
