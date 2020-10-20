#include "common/Log_c.h"
#include "common/Hardware/Sound_c.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "common/Hardware/Timer_c.h"

static const uint MIN_TIME = 500;


void HelpLong(void)
{
    SHOW_HELP_HINTS++;
    gStringForHint = 0;
    gItemHint = 0;
}

void Channel0Long(void)
{
    Menu::LongPressureButton(Key::ChannelA);
}

void Channel1Long(void)
{
    Menu::LongPressureButton(Key::ChannelB);
}

void TimeLong(void)
{
    Menu::LongPressureButton(Key::Time);
}

void TrigLong(void)
{
    if (MODE_LONG_PRESS_TRIG_IS_LEVEL0)
    {
        Menu::LongPressureButton(Key::Synchronization);
    }
    else
    {
        FPGA::FindAndSetTrigLevel();
    }
}

void StartDown()                        // Key::Start
{
    if (MODE_WORK_IS_DIRECT)
    {
        Menu::PressButton(Key::Start);
    }
}

void PowerDown()                        // B_Power
{
    ((Page *)Menu::OpenedItem())->ShortPressOnItem(0);
    Settings::Save();
    Log_DisconnectLoggerUSB();
    Panel::TransmitData(0x04);           // Посылаем команду выключения
}

void MenuLong() 
{
    Menu::LongPressureButton(Key::Menu);
}

void F1Long()
{
    Menu::LongPressureButton(Key::F1);
}

void F2Long()
{
    Menu::LongPressureButton(Key::F2);
}

void F3Long()
{
    Menu::LongPressureButton(Key::F3);
}

void F4Long()
{
    Menu::LongPressureButton(Key::F4);
}

void F5Long()
{
    Menu::LongPressureButton(Key::F5);
}

//****************************************************************************************************************
int CalculateCount(int *prevTime)
{
    uint time = gTimerMS;
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

bool CanChangeTShift(int16 tShift)
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
    else if (gTimerMS - time > MIN_TIME)
    {
        time = 0;
        return true;
    }
    return false;
}

bool CanChangeRShiftOrTrigLev(TrigSource::E channel, int16 rShift)
{
    static uint time[3] = {0, 0, 0};
    if (rShift == RShiftZero)
    {
        time[channel] = gTimerMS;
        return true;
    }
    else if (time[channel] == 0)
    {
        return true;
    }
    else if (gTimerMS - time[channel] > MIN_TIME)
    {
        time[channel] = 0;
        return true;
    }
    return false;
}

void ChangeRShift(int *prevTime, void(*f)(Channel::E, int16), Channel::E chan, int16 relStep)
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

void ChangeTrigLev(int *prevTime, void(*f)(TrigSource::E, int16), TrigSource::E trigSource, int16 relStep)
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

void ChangeTShift(int *prevTime, void(*f)(int), int16 relStep)
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

void ChangeShiftScreen(int *prevTime, void(*f)(int), int16 relStep)
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

static void SetRShift(Channel::E ch, int16 rShift)
{
    FPGA::SetRShift(ch, rShift);
}

void RShiftLeftA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::A, -STEP_RSHIFT);
}

void RShiftRightA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::A, +STEP_RSHIFT);
}

void RShiftLeftB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::B, -STEP_RSHIFT);
}

void RShiftRightB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, SetRShift, Channel::B, +STEP_RSHIFT);
}

static void SetTrigLev(TrigSource::E ch, int16 trigLev)
{
    FPGA::SetTrigLev(ch, trigLev);
}

void TrigLevLeft()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, SetTrigLev, TRIG_SOURCE, -STEP_RSHIFT);
}

void TrigLevRight()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, SetTrigLev, TRIG_SOURCE, +STEP_RSHIFT);
}

static void ShiftScreen(int shift)
{
    Display::ShiftScreen(shift);
}

static void SetTShift(int tShift)
{
    FPGA::SetTShift(tShift);
}

void XShift(int delta)
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

void TShiftLeft()
{
    XShift(-1);
}

void TShiftRight()
{
    XShift(1);
}

void RangeLeftA()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeIncrease(Channel::A);
}

void RangeRightA()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeDecrease(Channel::A);
}

void RangeLeftB()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeIncrease(Channel::B);
}

void RangeRightB()
{
    Sound::RegulatorSwitchRotate();
    FPGA::RangeDecrease(Channel::B);
}

void TBaseLeft()
{
    Sound::RegulatorSwitchRotate();
    FPGA::TBaseIncrease();
}

void TBaseRight()
{
    Sound::RegulatorSwitchRotate();
    FPGA::TBaseDecrease();
}

void SetLeft()
{
    Menu::RotateRegSetLeft();
}

void SetRight()
{
    Menu::RotateRegSetRight();
}
