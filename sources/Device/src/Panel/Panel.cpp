#include "defines.h"
#include "common/Log_c.h"
#include "common/Hardware/Sound_c.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/HelpContent.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
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

static const uint MIN_TIME = 500;

static Key::E pressedKey = Key::None;
volatile static Key::E pressedButton = Key::None;         // Это используется для отслеживания нажатой кнопки при отключенной панели
static uint16 dataTransmitted[MAX_DATA] = {0x00};
static uint16 numDataForTransmitted = 0;

bool Panel::isRunning = true;


static void HelpLong();
static void ChannelLongA();
static void ChannelLongB();
static void TimeLong();
static void TrigLong();
static void StartDown();
static void PowerDown();
static void F1Long();
static void F2Long();
static void F3Long();
static void F4Long();
static void F5Long();
static void MenuLong();
static void RShiftLeftA();
static void RShiftRightA();
static void RShiftLeftB();
static void RShiftRightB();
static void TrigLevLeft();
static void TrigLevRight();
static void TShiftLeft();
static void TShiftRight();
static void RangeLeftA();
static void RangeRightA();
static void RangeLeftB();
static void RangeRightB();
static void TBaseLeft();
static void TBaseRight();
static void SetLeft();
static void SetRight();


static void(*funcOnKeyDown[Key::Count])() =
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

static void (*funcOnKeyUp[Key::Count])() =
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

static void (*funcOnLongPressure[Key::Count])()    =
{
    0,
    F1Long,         // Key::F1
    F2Long,         // Key::F2
    F3Long,         // Key::F3
    F4Long,         // Key::F4
    F5Long,         // Key::F5
    ChannelLongA,   // Key::ChannelA
    ChannelLongB,   // Key::ChannelB
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

static void (*funculatorLeft[Key::Count])()    =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RangeLeftA,     // R_Range0
    RangeLeftB,     // R_Range1
    RShiftLeftA,    // R_RShift0
    RShiftLeftB,    // R_RShift1
    TBaseLeft,      // R_TBase
    TShiftLeft,     // R_TShift
    TrigLevLeft,    // R_TrigLev
    SetLeft         // Key::Setting
};
static void (*funculatorRight[Key::Count])() =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RangeRightA,    // R_Range0
    RangeRightB,    // R_Range1
    RShiftRightA,   // R_RShift0
    RShiftRightB,   // R_RShift1
    TBaseRight,     // R_TBase
    TShiftRight,    // R_TShift
    TrigLevRight,   // R_TrigLev
    SetRight        // Key::Setting
};



Key::E ButtonIsRelease(uint16 code) //-V2506
{
    Key::E key = Key::FromCode(code);
    Action::E action = Action::FromCode(code);

    if (Key::IsButton(key) && (action == Action::Up))
    {
        return key;
    }

    return Key::None;
}


Key::E ButtonIsPress(uint16 code) //-V2506
{
    Key::E key = Key::FromCode(code);
    Action::E action = Action::FromCode(code);

    if (Key::IsButton(key) && (action == Action::Down))
    {
        return key;
    }

    return Key::None;
}


Key::E ButtonIsLong(uint16 code) //-V2506
{
    Key::E key = Key::FromCode(code);
    Action::E action = Action::FromCode(code);

    if (Key::IsButton(key) && (action == Action::Long))
    {
        return key;
    }

    return Key::None;
}


Key::E RegulatorLeft(uint16 code) //-V2506
{
    Key::E key = Key::FromCode(code);
    Action::E action = Action::FromCode(code);

    if (Key::IsGovernor(key) && (action == Action::RotateLeft))
    {
        return key;
    }

    return Key::None;
}


Key::E RegulatorRight(uint16 code) //-V2506
{
    Key::E key = Key::FromCode(code);
    Action::E action = Action::FromCode(code);

    if (Key::IsGovernor(key) && (action == Action::RotateRight))
    {
        return key;
    }

    return Key::None;
}


bool Panel::ProcessingCommandFromPIC(uint16 code) //-V2506
{
    Key::E releaseButton = ButtonIsRelease(code);
    Key::E pressButton = ButtonIsPress(code);
    Key::E longButton = ButtonIsLong(code);
    Key::E regLeft = RegulatorLeft(code);
    Key::E regRight = RegulatorRight(code);

    if (pressButton != Key::None)
    {
        pressedButton = pressButton;
    }

    if(!Panel::isRunning)
    {
        return true;
    }

    if(releaseButton != Key::None)
    {
        Menu::Event::ReleaseButton(releaseButton);
        funcOnKeyUp[releaseButton]();
        if(pressedKey != Key::None)
        {
            Menu::Event::ShortPressureButton(releaseButton);
            pressedKey = Key::None;
        }
    }
    else if(pressButton != Key::None)
    {
        funcOnKeyDown[pressButton]();
        Menu::Event::PressButton(pressButton);
        pressedKey = pressButton;
    }
    else if (longButton != Key::None)
    {
        funcOnLongPressure[longButton]();
        pressedKey = Key::None;
    }
    else if(regLeft != Key::None)
    {
        funculatorLeft[regLeft](); //-V557
    }
    else if(regRight != Key::None) //-V2516
    {
        funculatorRight[regRight]();
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
        else if(gTimerMS - timeEnable > 100) //-V2516
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

uint16 Panel::NextData() //-V2506
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
    Panel::isRunning = false;
}

void Panel::Enable()
{
    Panel::isRunning = true;
}


void Panel::Init()
{
    // Лампочка УСТАНОВКА  pinLED

    Panel::EnableLEDRegSet(false);
}

void Panel::EnableLEDRegSet(bool enable)
{
    Pin::LED.Write(enable ? 1 : 0);
}

Key::E Panel::WaitPressingButton()
{
    pressedButton = Key::None;
    while (pressedButton == Key::None) {};
    return pressedButton;
}


static void HelpLong()
{
    Menu::showHelpHints = !Menu::showHelpHints;
    PageHelpContent::stringForHint = 0;
    PageHelpContent::itemHint = 0;
}


static void ChannelLongA()
{
    Menu::Event::LongPressureButton(Key::ChannelA);
}


static void ChannelLongB()
{
    Menu::Event::LongPressureButton(Key::ChannelB);
}


static void TimeLong()
{
    Menu::Event::LongPressureButton(Key::Time);
}


static void TrigLong()
{
    if (MODE_LONG_PRESS_TRIG_IS_LEVEL0)
    {
        Menu::Event::LongPressureButton(Key::Synchronization);
    }
    else
    {
        FPGA::FindAndSetTrigLevel();
    }
}


static void StartDown()
{
    if (MODE_WORK_IS_DIRECT)
    {
        Menu::Event::PressButton(Key::Start);
    }
}


static void PowerDown()
{
    ((Page *)Menu::OpenedItem())->ShortPressOnItem(0); //-V2533
    Settings::Save();
    Log_DisconnectLoggerUSB();
    Panel::TransmitData(0x04);           // Посылаем команду выключения
}


static void MenuLong()
{
    Menu::Event::LongPressureButton(Key::Menu);
}


static void F1Long()
{
    Menu::Event::LongPressureButton(Key::F1);
}


static void F2Long()
{
    Menu::Event::LongPressureButton(Key::F2);
}


static void F3Long()
{
    Menu::Event::LongPressureButton(Key::F3);
}


static void F4Long()
{
    Menu::Event::LongPressureButton(Key::F4);
}


static void F5Long()
{
    Menu::Event::LongPressureButton(Key::F5);
}


static bool CanChangeRShiftOrTrigLev(TrigSource::E channel, int16 rShift) //-V2506
{
    static uint time[3] = { 0, 0, 0 };
    if (rShift == RShiftZero)
    {
        time[channel] = gTimerMS;
        return true;
    }
    else if (time[channel] == 0)
    {
        return true;
    }
    else if (gTimerMS - time[channel] > MIN_TIME) //-V2516
    {
        time[channel] = 0;
        return true;
    }
    return false;
}


int CalculateCount(int *prevTime) //-V2506
{
    uint time = gTimerMS;
    uint delta = time - *prevTime;
    *prevTime = (int)time; //-V2533

    if (delta > 75)
    {
        return 1;
    }
    else if (delta > 50)
    {
        return 2;
    }
    else if (delta > 25) //-V2516
    {
        return 3;
    }
    return 4;
}


static void ChangeRShift(int *prevTime, void(*f)(Channel::E, int16), Channel::E chan, int16 relStep)
{
    if (ENUM_ACCUM_IS_NONE)
    {
        FPGA::TemporaryPause();
    }
    int count = CalculateCount(prevTime);
    int rShiftOld = SET_RSHIFT(chan);
    int rShift = SET_RSHIFT(chan) + relStep * count;
    if ((rShiftOld > RShiftZero && rShift < RShiftZero) || (rShiftOld < RShiftZero && rShift > RShiftZero))
    {
        rShift = RShiftZero;
    }
    if (CanChangeRShiftOrTrigLev((TrigSource::E)chan, (int16)rShift)) //-V2533
    {
        Sound::RegulatorShiftRotate();
        f(chan, (int16)rShift); //-V2533
    }
}


static void SetRShift(Channel::E ch, int16 rShift)
{
    FPGA::SetRShift(ch, rShift);
}


static void RShiftLeftA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::A, -STEP_RSHIFT);
}


static void RShiftRightA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::A, +STEP_RSHIFT);
}


static void RShiftLeftB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::B, -STEP_RSHIFT);
}


static void RShiftRightB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::B, +STEP_RSHIFT);
}


static void ChangeTrigLev(int *prevTime, void(*f)(TrigSource::E, int16), TrigSource::E trigSource, int16 relStep)
{
    int count = CalculateCount(prevTime);
    int trigLevOld = TRIG_LEVEL(trigSource);
    int trigLev = TRIG_LEVEL(trigSource) + relStep * count;
    if ((trigLevOld > TrigLevZero && trigLev < TrigLevZero) || (trigLevOld < TrigLevZero && trigLev > TrigLevZero))
    {
        trigLev = TrigLevZero;
    }
    if (CanChangeRShiftOrTrigLev(trigSource, (int16)trigLev)) //-V2533
    {
        Sound::RegulatorShiftRotate();
        f(trigSource, (int16)trigLev); //-V2533
    }
}


static void SetTrigLev(TrigSource::E ch, int16 trigLev)
{
    FPGA::SetTrigLev(ch, trigLev);
}


static void TrigLevLeft()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, SetTrigLev, TRIG_SOURCE, -STEP_RSHIFT);
}


static void TrigLevRight()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, SetTrigLev, TRIG_SOURCE, +STEP_RSHIFT);
}


static void SetTShift(int tShift)
{
    FPGA::SetTShift(tShift);
}


static void ShiftScreen(int shift)
{
    Display::ShiftScreen(shift);
}


static void ChangeShiftScreen(int *prevTime, void(*f)(int), int16 relStep)
{
    int count = CalculateCount(prevTime);
    float step = static_cast<float>(relStep * count);
    if (step < 0) //-V2564
    {
        if (step > -1) //-V2564
        {
            step = -1; //-V2564
        }
    }
    else if (step < 1) //-V2516 //-V2564
    {
        step = 1; //-V2564
    }
    f(static_cast<int>(step));
}


static bool CanChangeTShift(int16 tShift) //-V2506
{
    static uint time = 0;
    if (tShift == 0)
    {
        time = gTimerMS;
        return true;
    }
    else if (time == 0)
    {
        return true;
    }
    else if (gTimerMS - time > MIN_TIME) //-V2516
    {
        time = 0;
        return true;
    }
    return false;
}


static void ChangeTShift(int *prevTime, void(*f)(int), int16 relStep)
{
    int count = CalculateCount(prevTime);
    int tShiftOld = TSHIFT;
    float step = static_cast<float>(relStep * count);
    if (step < 0) //-V2564
    {
        if (step > -1) //-V2564
        {
            step = -1; //-V2564
        }
    }
    else
    {
        if (step < 1) //-V2564
        {
            step = 1; //-V2564
        }
    }

    int16 tShift = static_cast<int16>(TSHIFT + step); //-V2564
    if (((tShiftOld > 0) && (tShift < 0)) || (tShiftOld < 0 && tShift > 0))
    {
        tShift = 0;
    }
    if (CanChangeTShift(tShift))
    {
        Sound::RegulatorShiftRotate();
        f(tShift);
    }
}


static void XShift(int delta)
{
    static int prevTime = 0;
    if (!FPGA::IsRunning() || TIME_DIV_XPOS_IS_SHIFT_IN_MEMORY)
    {
        if (!ENUM_POINTS_IS_281)
        {
            ChangeShiftScreen(&prevTime, ShiftScreen, static_cast<int16>(2 * delta));
        }
    }
    else
    {
        ChangeTShift(&prevTime, SetTShift, (int16)delta); //-V2533
    }
}


static void TShiftLeft()
{
    XShift(-1);
}


static void TShiftRight()
{
    XShift(1);
}


static void RangeLeftA()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeIncrease(Channel::A);
}


static void RangeRightA()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeDecrease(Channel::A);
}


static void RangeLeftB()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeIncrease(Channel::B);
}


static void RangeRightB()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeDecrease(Channel::B);
}


static void TBaseLeft()
{
    Sound::RegulatorSwitchRotate();
    FPGA::TBaseIncrease();
}


static void TBaseRight()
{
    Sound::RegulatorSwitchRotate();
    FPGA::TBaseDecrease();
}


static void SetLeft()
{
    Menu::Event::RotateRegSetLeft();
}


static void SetRight()
{
    Menu::Event::RotateRegSetRight();
}
