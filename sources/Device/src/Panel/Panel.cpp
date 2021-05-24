// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Mutex_.h"
#include "common/Utils/Containers/Queue_.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/HelpContent.h"
#include "Panel/Panel.h"
#include <cstdio>
#include <cstring>


#define MAX_DATA            20

inline void E() {};

static const uint MIN_TIME = 500;

static Key::E pressedKey = Key::None;
volatile static Key::E pressedButton = Key::None;         // Это используется для отслеживания нажатой кнопки при
                                                          // отключенной панели
bool Panel::isRunning = true;

static Queue<uint8> data_for_panel;


extern LED       led_Trig(TypeLED::Trig);
extern RegSetLED led_RegSet(TypeLED::RegSet);
extern LED       led_ChA(TypeLED::ChanA);
extern LED       led_ChB(TypeLED::ChanB);
extern LED       led_Power(TypeLED::Power);


struct ReceivedBuffer
{
    ReceivedBuffer() : pointer(0)
    {
        std::memset(buffer, 0, SIZE);
    }

    void Push(uint8 byte)
    {
        if (pointer < SIZE)
        {
            buffer[pointer++] = byte;
        }
    }

    int Size() const
    {
        return pointer;
    }

    void Clear()
    {
        pointer = 0;
    }

    uint8 &operator[](int n)
    {
        if (n > (pointer - 1))
        {
            return buffer[0];
        }

        return buffer[n];
    }

    uint8 *Data() { return buffer; }

    Mutex mutex;    // При работе с буфером главный цикл лочит этот мьютекс, а прерывание проверяет, не залочен ли он

private:

    static const int SIZE = 20;
    uint8 buffer[SIZE];
    int pointer;
};


static ReceivedBuffer received_buffer;      // Прерывание от панели сюда складывает принятые данные
static Queue<uint8> queue;                  // А здесь идёт обработка принятных данных


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
    E,              // Key::None
    E,              // Key::F1
    E,              // Key::F2
    E,              // Key::F3
    E,              // Key::F4
    E,              // Key::F5
    E,              // Key::ChannelA
    E,              // Key::ChannelB
    E,              // Key::Time
    E,              // Key::Trig
    E,              // Key::Cursors
    E,              // Key::Measures
    E,              // Key::Display
    E,              // Key::Help
    StartDown,      // Key::Start
    E,              // Key::Memory                                         
    E,              // Key::Service
    E,              // Key::Menu
    PowerDown,      // Key::Power
    E,              // Key::RangeA
    E,              // Key::RangeB
    E,              // Key::RShiftA
    E,              // Key::RShiftB
    E,              // Key::TBase
    E,              // Key::TShift
    E,              // Key::TrigLev
    E               // Key::Setting
};

static const pFuncVV funcOnKeyUp[Key::Count] =
{
    E,              // Key::None
    E,              // Key::F1
    E,              // Key::F2
    E,              // Key::F3
    E,              // Key::F4
    E,              // Key::F5
    E,              // Key::ChannelA
    E,              // Key::ChannelB
    E,              // Key::Time
    E,              // Key::Trig
    E,              // Key::Cursors
    E,              // Key::Measures
    E,              // Key::Display
    E,              // Key::Help
    E,              // Key::Start
    E,              // Key::Memory
    E,              // Key::Service
    E,              // Key::Menu
    E,              // Key::Power
    E,              // Key::RangeA
    E,              // Key::RangeB
    E,              // Key::RShiftA
    E,              // Key::RShiftB
    E,              // Key::TBase
    E,              // Key::TShift
    E,              // Key::TrigLev
    E               // Key::Setting
};

static const pFuncVV funcOnLongPressure[Key::Count] =
{
    E,              // Key::None
    F1Long,         // Key::F1
    F2Long,         // Key::F2
    F3Long,         // Key::F3
    F4Long,         // Key::F4
    F5Long,         // Key::F5
    ChannelLongA,   // Key::ChannelA
    ChannelLongB,   // Key::ChannelB
    TimeLong,       // Key::Time
    TrigLong,       // Key::Trig
    E,              // Key::Cursors
    E,              // Key::Measures
    E,              // Key::Display
    HelpLong,       // Key::Help
    E,              // Key::Start
    E,              // Key::Memory
    E,              // Key::Service
    MenuLong,       // Key::Menu
    E,              // Key::Power
    E,              // Key::RangeA
    E,              // Key::RangeB
    E,              // Key::RShiftA
    E,              // Key::RShiftB
    E,              // Key::TBase
    E,              // Key::TShift
    E,              // Key::TrigLev
    E               // Key::Setting
};

static const pFuncVV funculatorLeft[Key::Count] =
{
    E,              // Key::None
    E,              // Key::F1
    E,              // Key::F2
    E,              // Key::F3
    E,              // Key::F4
    E,              // Key::F5
    E,              // Key::ChannelA
    E,              // Key::ChannelB
    E,              // Key::Time
    E,              // Key::Trig
    E,              // Key::Cursors
    E,              // Key::Measures
    E,              // Key::Display
    E,              // Key::Help
    E,              // Key::Start
    E,              // Key::Memory
    E,              // Key::Service
    E,              // Key::Menu
    E,              // Key::Power
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
    E,              // Key::None
    E,              // Key::F1
    E,              // Key::F2
    E,              // Key::F3
    E,              // Key::F4
    E,              // Key::F5
    E,              // Key::ChannelA
    E,              // Key::ChannelB
    E,              // Key::Time
    E,              // Key::Trig
    E,              // Key::Cursors
    E,              // Key::Measures
    E,              // Key::Display
    E,              // Key::Help
    E,              // Key::Start
    E,              // Key::Memory
    E,              // Key::Service
    E,              // Key::Menu
    E,              // Key::Power
    RangeRightA,    // Key::RangeA
    RangeRightB,    // Key::RangeB
    RShiftRightA,   // Key::RShiftA
    RShiftRightB,   // Key::RShiftB
    TBaseRight,     // Key::TBase
    TShiftRight,    // Key::TShift
    TrigLevRight,   // Key::TrigLev
    SetRight        // Key::Setting
};


static void CopyData()
{
    received_buffer.mutex.Lock();

    for (int i = 0; i < received_buffer.Size(); i++)
    {
        queue.Push(received_buffer[i]);
    }

    received_buffer.Clear();

    received_buffer.mutex.Unlock();
}


void Panel::Update()
{
    CopyData();

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
        Menu::Handlers::ReleaseKey(key);
        funcOnKeyUp[key]();
        if (pressedKey != Key::None)
        {
            Menu::Handlers::ShortPressureKey(key);
            pressedKey = Key::None;
        }
    }
    else if (action.IsDown())
    {
        funcOnKeyDown[key]();
        Menu::Handlers::PressKey(key);
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


void Panel::CallbackOnReceiveSPI5(uint8 *data, uint size)
{
    static ReceivedBuffer buffer;

    for (uint i = 0; i < size; i++)              // Сначала сохраняем данные в промежуточный буфер
    {
        buffer.Push(data[i]);
    }

    if (!received_buffer.mutex.IsLocked())      // А затем, если основной буфер не заблокирован
    {
        for (int i = 0; i < buffer.Size(); i++)
        {
            received_buffer.Push(buffer[i]);    // сохраняем в нём все несохранённые данные
        }

        buffer.Clear();                         // И не забываем очистить прожежуточный
    }
}


void Panel::DisablePower()
{
    if (TIME_MS > 1000)
    {
        Panel::TransmitData(TypeLED::Power);
    }
}


void Panel::TransmitData(uint8 data)
{
    if(data_for_panel.Size() > MAX_DATA)
    {
        LOG_WRITE("Не могу послать в панель - мало места");
    }
    else
    {
        data_for_panel.Push(data);
    }
}


uint8 Panel::NextData()
{
    return data_for_panel.Front();
}


void Panel::DisableInput()
{
    Panel::isRunning = false;
}


void Panel::EnableInput()
{
    Panel::isRunning = true;
}


void Panel::Init()
{
    led_RegSet.Disable();
    led_Trig.Disable();
    PageChannelA::OnChanged_Input(true);
    PageChannelB::OnChanged_Input(true);
}

Key::E Panel::WaitPressingButton()
{
    pressedButton = Key::None;

    while (pressedButton == Key::None)
    {
        Update();
    };

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
    Menu::Handlers::LongPressureKey(Key::ChannelA);
}


static void ChannelLongB()
{
    Menu::Handlers::LongPressureKey(Key::ChannelB);
}


static void TimeLong()
{
    Menu::Handlers::LongPressureKey(Key::Time);
}


static void TrigLong()
{
    if (ModeLongPressTrig::IsLevel0())
    {
        Menu::Handlers::LongPressureKey(Key::Trig);
    }
    else
    {
        TrigLev::FindAndSet();
    }
}


static void StartDown()
{
    if (set.memory.mode_work.IsDirect())
    {
        Menu::Handlers::PressKey(Key::Start);
    }
}


static void PowerDown()
{
    ((Page *)Item::Opened())->ShortPressOnItem(0);

    Panel::DisablePower();
}


static void MenuLong()
{
    Menu::Handlers::LongPressureKey(Key::Menu);
}


static void F1Long()
{
    Menu::Handlers::LongPressureKey(Key::F1);
}


static void F2Long()
{
    Menu::Handlers::LongPressureKey(Key::F2);
}


static void F3Long()
{
    Menu::Handlers::LongPressureKey(Key::F3);
}


static void F4Long()
{
    Menu::Handlers::LongPressureKey(Key::F4);
}


static void F5Long()
{
    Menu::Handlers::LongPressureKey(Key::F5);
}


static bool CanChangeRShiftOrTrigLev(TrigSource::E channel, int16 rShift)
{
    static uint time[3] = { 0, 0, 0 };
    if (rShift == RShift::ZERO)
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


static void ChangeRShift(int *prevTime, void(*f)(const Channel &, int16), const Channel &ch, int16 relStep)
{
    if (set.display.enum_accum.IsNone())
    {
        FPGA::TemporaryPause();
    }
    int count = CalculateCount(prevTime);
    int rShiftOld = set.chan[ch].rshift;
    int rShift = set.chan[ch].rshift + relStep * count;
    if ((rShiftOld > RShift::ZERO && rShift < RShift::ZERO) || (rShiftOld < RShift::ZERO && rShift > RShift::ZERO))
    {
        rShift = RShift::ZERO;
    }
    if (CanChangeRShiftOrTrigLev((TrigSource::E)ch.value, (int16)rShift))
    {
        Sound::RegulatorShiftRotate();
        f(ch, (int16)rShift);
    }
}


static void RShiftLeftA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChA, -RShift::STEP());
}


static void RShiftRightA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChA, +RShift::STEP());
}


static void RShiftLeftB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChB, -RShift::STEP());
}


static void RShiftRightB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChB, +RShift::STEP());
}


static void ChangeTrigLev(int *prevTime, void(*f)(TrigSource::E, int16), TrigSource::E trigSource, int16 relStep)
{
    int count = CalculateCount(prevTime);
    int trigLevOld = TrigLev::Get(trigSource);
    int trigLev = TrigLev::Get(trigSource) + relStep * count;

    if ((trigLevOld > TrigLev::ZERO && trigLev < TrigLev::ZERO) ||
        (trigLevOld < TrigLev::ZERO && trigLev > TrigLev::ZERO))
    {
        trigLev = TrigLev::ZERO;
    }

    if (CanChangeRShiftOrTrigLev(trigSource, (int16)trigLev))
    {
        Sound::RegulatorShiftRotate();
        f(trigSource, (int16)trigLev);
    }
}


static void TrigLevLeft()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, TrigLev::Set, TrigSource::Get(), -RShift::STEP());
}


static void TrigLevRight()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, TrigLev::Set, TrigSource::Get(), +RShift::STEP());
}


static void ChangeShiftScreen(int *prevTime, void(*f)(int), int16 relStep)
{
    int count = CalculateCount(prevTime);
    float step = (float)(relStep * count);
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
    f((int)(step));
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
    int tShiftOld = set.time.shift;
    float step = (float)(relStep * count);
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

    int16 tShift = (int16)(set.time.shift + step);
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
    if (!FPGA::IsRunning() || FunctionTime::IsShiftInMemory())
    {
        if (set.memory.enum_points_fpga.PointsInChannel() != EnumPointsFPGA::_281)
        {
            ChangeShiftScreen(&prevTime,  Display::ShiftScreen, (int16)(2 * delta));
        }
    }
    else
    {
        ChangeTShift(&prevTime, TShift::Set, (int16)delta);
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
    Range::Increase(ChA);
}


static void RangeRightA()
{
    Sound::RegulatorSwitchRotate();
    Range::Decrease(ChA);
}


static void RangeLeftB()
{
    Sound::RegulatorSwitchRotate();
    Range::Increase(ChB);
}


static void RangeRightB()
{
    Sound::RegulatorSwitchRotate();
    Range::Decrease(ChB);
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
    Menu::Handlers::RegulatorSet(Action::RotateLeft);
}


static void SetRight()
{
    Menu::Handlers::RegulatorSet(Action::RotateRight);
}


LED::LED(TypeLED::E _led) : led(_led)
{
}


void LED::Disable()
{
    SwitchToState(false);
}


void LED::SwitchToState(bool enabled)
{
    if (led == TypeLED::Trig)
    {
        static uint timeEnable = 0;
        static bool first = true;
        static bool fired = false;
        if (first)
        {
            Panel::TransmitData((uint8)led);
            TrigLev::exist_impulse = false;
            timeEnable = TIME_MS;
            first = false;
        }

        if (enabled)
        {
            timeEnable = TIME_MS;
        }

        if (enabled != fired)
        {
            if (enabled)
            {
                Panel::TransmitData((uint8)(led | 0x80));
                TrigLev::exist_impulse = true;
                fired = true;
            }
            else if (TIME_MS - timeEnable > 100)
            {
                Panel::TransmitData((uint8)(led));
                TrigLev::exist_impulse = false;
                fired = false;
            }
        }
    }
    else
    {
        uint8 value = (uint8)led;

        if (enabled)
        {
            value |= 0x80;
        }

        Panel::TransmitData(value);
    }
}


void RegSetLED::Switch()
{
    static bool first = true;
    static bool prevState = false;  // true - горит, false - не горит

    bool state = NeedForFire();

    if (first)
    {
        first = false;
        led_RegSet.SwitchToState(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        led_RegSet.SwitchToState(state);
        prevState = state;
    }
}


bool RegSetLED::NeedForFire()
{
    if (!Menu::IsShown())
    {
        return false;
    }
    NamePage::E name = Menu::GetNameOpenedPage();

    if (name == NamePage::SB_MeasTuneMeas && set.measures.number.Is1() && !PageMeasures::choiceMeasuresIsActive)
    {
        return false;
    }

    if (
         name == NamePage::SB_MathCursorsFFT                               ||
         name == NamePage::SB_MeasTuneMeas                                 ||
         name == NamePage::SB_MemLatest                                    ||
         name == NamePage::SB_MemInt                                       ||
        (name == NamePage::SB_MathFunction && !ModeDrawMath::IsDisabled()) ||
        (name == NamePage::SB_Curs && Cursors::NecessaryDraw())
        )
    {
        return true;
    }

    if (Menu::CurrentItem()->IsGovernor() || Menu::CurrentItem()->IsChoiceReg())
    {
        return true;
    }

    if (Item::Opened()->IsChoice() ||
        (Item::Opened()->IsPage() && (Item::Opened()->ReinterpretToPage()->NumSubPages() > 1)))
    {
        return true;
    }

    return false;
}
