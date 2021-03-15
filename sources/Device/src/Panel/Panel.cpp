#include "defines.h"
#include "common/Log_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Queue_.h"
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


static Queue <uint8>queue;


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


static const pFuncVV funcOnKeyDown[Key::Count] =
{    
    EmptyFuncVV,    // Key::None
    EmptyFuncVV,    // Key::F1
    EmptyFuncVV,    // Key::F2
    EmptyFuncVV,    // Key::F3
    EmptyFuncVV,    // Key::F4
    EmptyFuncVV,    // Key::F5
    EmptyFuncVV,    // Key::ChannelA
    EmptyFuncVV,    // Key::ChannelB
    EmptyFuncVV,    // Key::Time
    EmptyFuncVV,    // Key::Trig
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    EmptyFuncVV,    // Key::Help
    StartDown,      // Key::Start
    EmptyFuncVV,    // Key::Memory                                         
    EmptyFuncVV,    // Key::Service
    EmptyFuncVV,    // Key::Menu
    PowerDown,      // Key::Power
    EmptyFuncVV,    // Key::RangeA
    EmptyFuncVV,    // Key::RangeB
    EmptyFuncVV,    // Key::RShiftA
    EmptyFuncVV,    // Key::RShiftB
    EmptyFuncVV,    // Key::TBase
    EmptyFuncVV,    // Key::TShift
    EmptyFuncVV,    // Key::TrigLev
    EmptyFuncVV     // Key::Setting
};

static const pFuncVV funcOnKeyUp[Key::Count] =
{
    EmptyFuncVV,    // Key::None
    EmptyFuncVV,    // Key::F1
    EmptyFuncVV,    // Key::F2
    EmptyFuncVV,    // Key::F3
    EmptyFuncVV,    // Key::F4
    EmptyFuncVV,    // Key::F5
    EmptyFuncVV,    // Key::ChannelA
    EmptyFuncVV,    // Key::ChannelB
    EmptyFuncVV,    // Key::Time
    EmptyFuncVV,    // Key::Trig
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    EmptyFuncVV,    // Key::Help
    EmptyFuncVV,    // Key::Start
    EmptyFuncVV,    // Key::Memory
    EmptyFuncVV,    // Key::Service
    EmptyFuncVV,    // Key::Menu
    EmptyFuncVV,    // Key::Power
    EmptyFuncVV,    // Key::RangeA
    EmptyFuncVV,    // Key::RangeB
    EmptyFuncVV,    // Key::RShiftA
    EmptyFuncVV,    // Key::RShiftB
    EmptyFuncVV,    // Key::TBase
    EmptyFuncVV,    // Key::TShift
    EmptyFuncVV,    // Key::TrigLev
    EmptyFuncVV     // Key::Setting
};

static const pFuncVV funcOnLongPressure[Key::Count] =
{
    EmptyFuncVV,    // Key::None
    F1Long,         // Key::F1
    F2Long,         // Key::F2
    F3Long,         // Key::F3
    F4Long,         // Key::F4
    F5Long,         // Key::F5
    ChannelLongA,   // Key::ChannelA
    ChannelLongB,   // Key::ChannelB
    TimeLong,       // Key::Time
    TrigLong,       // Key::Trig
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    HelpLong,       // Key::Help
    EmptyFuncVV,    // Key::Start
    EmptyFuncVV,    // Key::Memory
    EmptyFuncVV,    // Key::Service
    MenuLong,       // Key::Menu
    EmptyFuncVV,    // Key::Power
    EmptyFuncVV,    // Key::RangeA
    EmptyFuncVV,    // Key::RangeB
    EmptyFuncVV,    // Key::RShiftA
    EmptyFuncVV,    // Key::RShiftB
    EmptyFuncVV,    // Key::TBase
    EmptyFuncVV,    // Key::TShift
    EmptyFuncVV,    // Key::TrigLev
    EmptyFuncVV     // Key::Setting
};

static const pFuncVV funculatorLeft[Key::Count] =
{
    EmptyFuncVV,    // Key::None
    EmptyFuncVV,    // Key::F1
    EmptyFuncVV,    // Key::F2
    EmptyFuncVV,    // Key::F3
    EmptyFuncVV,    // Key::F4
    EmptyFuncVV,    // Key::F5
    EmptyFuncVV,    // Key::ChannelA
    EmptyFuncVV,    // Key::ChannelB
    EmptyFuncVV,    // Key::Time
    EmptyFuncVV,    // Key::Trig
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    EmptyFuncVV,    // Key::Help
    EmptyFuncVV,    // Key::Start
    EmptyFuncVV,    // Key::Memory
    EmptyFuncVV,    // Key::Service
    EmptyFuncVV,    // Key::Menu
    EmptyFuncVV,    // Key::Power
    RangeLeftA,     // Key::RangeA
    RangeLeftB,     // Key::RangeB
    RShiftLeftA,    // Key::RShiftA
    RShiftLeftB,    // Key::RShiftB
    TBaseLeft,      // Key::TBase
    TShiftLeft,     // Key::TShift
    TrigLevLeft,    // Key::TrigLev
    SetLeft         // Key::Setting
};
static const pFuncVV funculatorRight[Key::Count] =
{
    EmptyFuncVV,    // Key::None
    EmptyFuncVV,    // Key::F1
    EmptyFuncVV,    // Key::F2
    EmptyFuncVV,    // Key::F3
    EmptyFuncVV,    // Key::F4
    EmptyFuncVV,    // Key::F5
    EmptyFuncVV,    // Key::ChannelA
    EmptyFuncVV,    // Key::ChannelB
    EmptyFuncVV,    // Key::Time
    EmptyFuncVV,    // Key::Trig
    EmptyFuncVV,    // Key::Cursors
    EmptyFuncVV,    // Key::Measures
    EmptyFuncVV,    // Key::Display
    EmptyFuncVV,    // Key::Help
    EmptyFuncVV,    // Key::Start
    EmptyFuncVV,    // Key::Memory
    EmptyFuncVV,    // Key::Service
    EmptyFuncVV,    // Key::Menu
    EmptyFuncVV,    // Key::Power
    RangeRightA,    // Key::RangeA
    RangeRightB,    // Key::RangeB
    RShiftRightA,   // Key::RShiftA
    RShiftRightB,   // Key::RShiftB
    TBaseRight,     // Key::TBase
    TShiftRight,    // Key::TShift
    TrigLevRight,   // Key::TrigLev
    SetRight        // Key::Setting
};



void Panel::Update()
{
    while (queue.Size() != 0 && queue[0] != 0xFF)
    {
        queue.Front();
    }

    if (queue.Size() < 3)
    {
        return;
    }

    queue.Front();
    Key::E key = (Key::E)queue.Front();
    Action action(queue.Front());

    if (action.IsDown())
    {
        pressedButton = key;
    }

    if (!Panel::isRunning)
    {
        return;
    }

    if (action.IsUp())
    {
        Menu::Event::ReleaseButton(key);
        funcOnKeyUp[key]();
        if (pressedKey != Key::None)
        {
            Menu::Event::ShortPressureButton(key);
            pressedKey = Key::None;
        }
    }
    else if (action.IsDown())
    {
        funcOnKeyDown[key]();
        Menu::Event::PressButton(key);
        pressedKey = key;
    }
    else if (action.IsLong())
    {
        funcOnLongPressure[key]();
        pressedKey = Key::None;
    }
    else if (action.IsRotateLeft())
    {
        funculatorLeft[key]();
    }
    else if (action.IsRotateRight())
    {
        funculatorRight[key]();
    }
}


void Panel::CallbackOnReceiveSPI5(uint8 *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        queue.Push(*data++);
    }

    char buffer[100] = { 0 };

    for (int i = 0; i < queue.Size(); i++)
    {
        char buf[10];
        std::sprintf(buf, "%d ", queue[i]);

        std::strcat(buffer, buf);
    }

    LOG_WRITE(buffer);
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
        timeEnable = TIME_MS;
        first = false;
    }

    if(enable)
    {
        timeEnable = TIME_MS;
    }

    if(enable != fired)
    {
        if(enable)
        {
            Panel::TransmitData(LED_TRIG_ENABLE);
            Display::EnableTrigLabel(true);
            fired = true;
        }
        else if(TIME_MS - timeEnable > 100)
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
        Menu::Event::LongPressureButton(Key::Trig);
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
    ((Page *)Item::Opened())->ShortPressOnItem(0);
    Settings::Save();
//\    Log_DisconnectLoggerUSB();
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


static bool CanChangeRShiftOrTrigLev(TrigSource::E channel, int16 rShift)
{
    static uint time[3] = { 0, 0, 0 };
    if (rShift == RShiftZero)
    {
        time[channel] = TIME_MS;
        return true;
    }
    else if (time[channel] == 0)
    {
        return true;
    }
    else if (TIME_MS - time[channel] > MIN_TIME)
    {
        time[channel] = 0;
        return true;
    }
    return false;
}


int CalculateCount(int *prevTime)
{
    uint time = TIME_MS;
    uint delta = time - *prevTime;
    *prevTime = (int)time;

    if (delta > 75)
    {
        return 1;
    }
    else if (delta > 50)
    {
        return 2;
    }
    else if (delta > 25)
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
    if (CanChangeRShiftOrTrigLev((TrigSource::E)chan, (int16)rShift))
    {
        Sound::RegulatorShiftRotate();
        f(chan, (int16)rShift);
    }
}


static void SetRShift(Channel::E ch, int16 rShift)
{
    RShift::Set(ch, rShift);
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
    if (CanChangeRShiftOrTrigLev(trigSource, (int16)trigLev))
    {
        Sound::RegulatorShiftRotate();
        f(trigSource, (int16)trigLev);
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
    if (step < 0)
    {
        if (step > -1)
        {
            step = -1;
        }
    }
    else if (step < 1)
    {
        step = 1;
    }
    f(static_cast<int>(step));
}


static bool CanChangeTShift(int16 tShift)
{
    static uint time = 0;
    if (tShift == 0)
    {
        time = TIME_MS;
        return true;
    }
    else if (time == 0)
    {
        return true;
    }
    else if (TIME_MS - time > MIN_TIME)
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
    if (step < 0)
    {
        if (step > -1)
        {
            step = -1;
        }
    }
    else
    {
        if (step < 1)
        {
            step = 1;
        }
    }

    int16 tShift = static_cast<int16>(TSHIFT + step);
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
        ChangeTShift(&prevTime, SetTShift, (int16)delta);
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
    Range::Increase(Channel::A);
}


static void RangeRightA()
{
    Sound::RegulatorSwitchRotate();
    Range::Decrease(Channel::A);
}


static void RangeLeftB()
{
    Sound::RegulatorSwitchRotate();
    Range::Increase(Channel::B);
}


static void RangeRightB()
{
    Sound::RegulatorSwitchRotate();
    Range::Decrease(Channel::B);
}


static void TBaseLeft()
{
    Sound::RegulatorSwitchRotate();
    TBase::Increase();
}


static void TBaseRight()
{
    Sound::RegulatorSwitchRotate();
    TBase::Decrease();
}


static void SetLeft()
{
    Menu::Event::RotateRegSetLeft();
}


static void SetRight()
{
    Menu::Event::RotateRegSetRight();
}
