#include "defines.h"
#include "Panel.h"
#include "Panel/Controls.h"
#include "common/Hardware/HAL/HAL.h"
#include "PanelFunctions.cpp"
#include "Utils/GlobalFunctions.h"
#include <cstdio>
#include <cstring>



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA            20

#define LED_CHAN0_ENABLE    129U
#define LED_CHAN0_DISABLE   1U
#define LED_CHAN1_ENABLE    130U
#define LED_CHAN1_DISABLE   2U
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4

static PanelButton::E pressedKey = PanelButton::Empty;
static PanelButton::E pressedButton = PanelButton::Empty;         // Это используется для отслеживания нажатой кнопки при отключенной панели
static uint16 dataTransmitted[MAX_DATA] = {0x00};
static uint16 numDataForTransmitted = 0;
static uint timePrevPressButton = 0;
static uint timePrevReleaseButton = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void(*funcOnKeyDown[PanelButton::Count])(void)    =
{    
    0,
    EmptyFuncVV,    // PanelButton::Channel0
    EmptyFuncVV,    // PanelButton::Service
    EmptyFuncVV,    // PanelButton::Channel1
    EmptyFuncVV,    // PanelButton::Display
    EmptyFuncVV,    // PanelButton::Time
    EmptyFuncVV,    // PanelButton::Memory                                         
    EmptyFuncVV,    // B_Sinchro
    StartDown,      // PanelButton::Start
    EmptyFuncVV,    // PanelButton::Cursors
    EmptyFuncVV,    // PanelButton::Measures
    PowerDown,      // B_Power
    EmptyFuncVV,    // PanelButton::Help
    EmptyFuncVV,    // PanelButton::Menu
    EmptyFuncVV,    // PanelButton::F1
    EmptyFuncVV,    // PanelButton::F2
    EmptyFuncVV,    // PanelButton::F3
    EmptyFuncVV,    // PanelButton::F4
    EmptyFuncVV     // PanelButton::F5
};

static void (*funcOnKeyUp[PanelButton::Count])(void)    =
{
    0,
    EmptyFuncVV,    // PanelButton::Channel0
    EmptyFuncVV,    // PanelButton::Service
    EmptyFuncVV,    // PanelButton::Channel1
    EmptyFuncVV,    // PanelButton::Display
    EmptyFuncVV,    // PanelButton::Time
    EmptyFuncVV,    // PanelButton::Memory
    EmptyFuncVV,    // B_Sinchro
    EmptyFuncVV,    // PanelButton::Start
    EmptyFuncVV,    // PanelButton::Cursors
    EmptyFuncVV,    // PanelButton::Measures
    EmptyFuncVV,    // B_Power
    EmptyFuncVV,    // PanelButton::Help
    EmptyFuncVV,    // PanelButton::Menu
    EmptyFuncVV,    // PanelButton::F1
    EmptyFuncVV,    // PanelButton::F2
    EmptyFuncVV,    // PanelButton::F3
    EmptyFuncVV,    // PanelButton::F4
    EmptyFuncVV     // PanelButton::F5
};

static void (*funcOnLongPressure[PanelButton::Count])(void)    =
{
    0,
    Channel0Long,   // PanelButton::Channel0
    EmptyFuncVV,    // PanelButton::Service
    Channel1Long,   // PanelButton::Channel1
    EmptyFuncVV,    // PanelButton::Display
    TimeLong,       // PanelButton::Time
    EmptyFuncVV,    // PanelButton::Memory
    TrigLong,       // B_Sinchro
    EmptyFuncVV,    // PanelButton::Start
    EmptyFuncVV,    // PanelButton::Cursors
    EmptyFuncVV,    // PanelButton::Measures
    EmptyFuncVV,    // B_Power
    HelpLong,       // PanelButton::Help
    MenuLong,       // PanelButton::Menu
    F1Long,         // PanelButton::F1
    F2Long,         // PanelButton::F2
    F3Long,         // PanelButton::F3
    F4Long,         // PanelButton::F4
    F5Long          // PanelButton::F5
};

static void (*funculatorLeft[Regulator::Set + 1])(void)    =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Range0Left,     // R_Range0
    RShift0Left,    // R_RShift0
    Range1Left,     // R_Range1
    RShift1Left,    // R_RShift1
    TBaseLeft,      // R_TBase
    TShiftLeft,     // R_TShift
    TrigLevLeft,    // R_TrigLev
    SetLeft         // Regulator::Set
};
static void (*funculatorRight[Regulator::Set + 1])(void) =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Range0Right,    // R_Range0
    RShift0Right,   // R_RShift0
    Range1Right,    // R_Range1
    RShift1Right,   // R_RShift1
    TBaseRight,     // R_TBase
    TShiftRight,    // R_TShift
    TrigLevRight,   // R_TrigLev
    SetRight        // Regulator::Set
};



PanelButton::E ButtonIsRelease(uint16 command)
{
    PanelButton::E button = PanelButton::Empty;
    if(command < PanelButton::Count && command > PanelButton::Empty)
    {
        if(gTimerMS - timePrevReleaseButton > 100)
        {
            button = (PanelButton::E)command;
            timePrevReleaseButton = gTimerMS;
        }
    }
    return button;
}

PanelButton::E ButtonIsPress(uint16 command)
{
    PanelButton::E button = PanelButton::Empty;
    if (((command & 0x7f) < PanelButton::Count) && ((command & 0x7f) > PanelButton::Empty))
    {
        if(gTimerMS - timePrevPressButton > 100)
        {
            button = (PanelButton::E)(command & 0x7f);
            timePrevPressButton = gTimerMS;
        }
    }
    return button;
}

Regulator::E RegulatorLeft(uint16 command)
{
    if(command >= 20 && command <= 27)
    {
        return (Regulator::E)command;
    }
    return Regulator::Empty;
}

Regulator::E RegulatorRight(uint16 command)
{
    if(((command & 0x7f) >= 20) && ((command & 0x7f) <= 27))
    {
        return (Regulator::E)(command & 0x7f);
    }
    return Regulator::Empty;
}

void OnTimerPressedKey()
{
    if(pressedKey != PanelButton::Empty)
    {
        void (*func)(void) = funcOnLongPressure[pressedKey];
        Menu::ReleaseButton(pressedKey);
        if(func != 0)
        {
            func();
        }
        pressedKey = PanelButton::Empty;
    }
    Timer::Disable(TypeTimer::PressKey);
}

bool Panel::ProcessingCommandFromPIC(uint16 command)
{
    static int allRecData = 0;

    PanelButton::E releaseButton = ButtonIsRelease(command);
    PanelButton::E pressButton = ButtonIsPress(command);
    Regulator::E regLeft = RegulatorLeft(command);
    Regulator::E regRight = RegulatorRight(command);

    if (pressButton)
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

    if(releaseButton)
    {
        Menu::ReleaseButton(releaseButton);
        funcOnKeyUp[releaseButton]();
        if(pressedKey != PanelButton::Empty)
        {
            Menu::ShortPressureButton(releaseButton);
            pressedKey = PanelButton::Empty;
        }
    }
    else if(pressButton)
    {
        funcOnKeyDown[pressButton]();
        Menu::PressButton(pressButton);
        pressedKey = pressButton;
        Timer::Enable(TypeTimer::PressKey, 500, OnTimerPressedKey);
    }
    else if(regLeft)
    {
        /*
        if (set.memory.modeWork == ModeWork_Direct || regLeft == Regulator::Set || regLeft == R_TShift)
        {
        */
            funculatorLeft[regLeft]();
        //}
    }
    else if(regRight)
    {
        /*
        if (set.memory.modeWork == ModeWork_Direct || regRight == Regulator::Set || regRight == R_TShift)
        {
        */
            funculatorRight[regRight]();
        //}
    }

    if ((command > Regulator::Set && command < (PanelButton::Empty + 1 + 128)) || (command > (Regulator::Set + 128)))
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

PanelButton::E Panel::WaitPressingButton()
{
    pressedButton = PanelButton::Empty;
    while (pressedButton == PanelButton::Empty) {};
    return pressedButton;
}
