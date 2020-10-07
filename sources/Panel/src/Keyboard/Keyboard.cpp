#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include <limits>


static Control commands[10];
static int pointer = 0;


#define NUM_SL 6
#define NUM_RL 8    

#define BUTTON_IS_PRESS(state)  ((state) == 0)

#define KEY(sl, rl) (keys[sl][rl])


struct KeyStruct
{
    KeyStruct(Key::E k) : key(k), timePress(0U) { }
    // Возвращает true, если нопка наодится в нажатом положении
    bool IsPressed() const { return (timePress != 0); }
    // Вовзращате true, если произошло длинное нажатие
    bool HappenedLongPressed() const { return (timePress == UINT_MAX); }

    bool IsValid() const { return (key != Key::None); }
    void Process(int sl, int rl);
    Key::E key;
    uint   timePress;   // Время нажатия кнопки
};


static KeyStruct keys[NUM_SL][NUM_RL] =     
//    RL0                   RL1            RL2           RL3           RL4          RL5           RL6           RL7
    {{Key::ChannelA,        Key::None,     Key::None,    Key::RangeA,  Key::None,   Key::None,    Key::RShiftA, Key::Menu},  // SL0
     {Key::ChannelB,        Key::None,     Key::None,    Key::RangeB,  Key::None,   Key::None,    Key::RShiftB, Key::F1},    // SL1
     {Key::Time,            Key::None,     Key::None,    Key::TBase,   Key::None,   Key::None,    Key::TShift,  Key::F2},    // SL2
     {Key::Synchronization, Key::None,     Key::None,    Key::TrigLev, Key::Start,  Key::None,    Key::None,    Key::F3},    // SL3
     {Key::Cursors,         Key::Measures, Key::Display, Key::Help,    Key::Memory, Key::Service, Key::None,    Key::F4},    // SL4
     {Key::None,            Key::None,     Key::None,    Key::Setting, Key::None,   Key::None,    Key::None,    Key::F5}};   // SL5

static Pin *sls[NUM_SL] = { &pinSL0, &pinSL1, &pinSL2, &pinSL3, &pinSL4, &pinSL5 };
static Pin *rls[NUM_RL] = { &pinRL0, &pinRL1, &pinRL2, &pinRL3, &pinRL4, &pinRL5, &pinRL6, &pinRL7 };

#define SET_SL(n)       sls[n]->Set();
#define RESET_SL(n)     sls[n]->Reset();
#define READ_RL(n)      rls[n]->Read()


struct GovernorStruct
{
    GovernorStruct(Key::E k, uint8 rlA_, uint8 rlB_, uint8 sl_) : key(k), rlA(rlA_), rlB(rlB_), sl(sl_), prevStateIsSame(false) { }
    void Process();
    Key::E key;
    uint8  rlA;
    uint8  rlB;
    uint8  sl;
    bool   prevStateIsSame;   // true, если предыдущие состояния одинаковы
};

#define NUM_GOVERNORS 8

static GovernorStruct governors[NUM_GOVERNORS] =
{
    GovernorStruct(Key::RangeA,  1, 2, 0),
    GovernorStruct(Key::RangeB,  1, 2, 1),
    GovernorStruct(Key::RShiftA, 4, 5, 0),
    GovernorStruct(Key::RShiftB, 4, 5, 1),
    GovernorStruct(Key::TBase,   1, 2, 2),
    GovernorStruct(Key::TShift,  4, 5, 2),
    GovernorStruct(Key::TrigLev, 1, 2, 3),
    GovernorStruct(Key::Setting, 1, 2, 5)
};


void Keyboard::Init()
{
    pinSL0.Set();
    pinSL1.Set();
    pinSL2.Set();
    pinSL3.Set();
    pinSL4.Set();
    pinSL5.Set();

    pointer = 0;

    HAL_TIM2::Init(&Keyboard::Update);
}


void Keyboard::Update()
{
    for (int sl = 0; sl < NUM_SL; sl++)
    {
        for (int rl = 0; rl < NUM_RL; rl++)
        {
            KEY(sl, rl).Process(sl, rl);
        }
    }

    for (int i = 0; i < NUM_GOVERNORS; i++)
    {
        governors[i].Process();
    }
}


void KeyStruct::Process(int sl, int rl)
{
    uint time = TIME_MS;

    SET_SL(sl);

    uint state = READ_RL(rl);

    RESET_SL(sl);

    if (IsValid())
    {
        if (IsPressed() && !HappenedLongPressed())                      // Если клавиша находится в нажатом положении
        {
            uint delta = time - timePress;
            if (delta > 500)                                            // Если прошло более 500 мс с момента нажатия -
            {
                timePress = UINT_MAX;
                Keyboard::Buffer::AppendEvent(key, Action::Long);       // это будет длинное нажатие
            }
            else if (delta > 100 &&                                     // Если прошло более 100 мс с момента нажатия
                     !BUTTON_IS_PRESS(state))                           // и сейчас кнопка находится в отжатом состоянии
            {
                timePress = UINT_MAX;                                   // То учитываем это в массиве
                Keyboard::Buffer::AppendEvent(key, Action::Up);         // И сохраняем отпускание кнопки в буфере команд
            }
        }
        else if (BUTTON_IS_PRESS(state) && !HappenedLongPressed())      // Если кнопка нажата
        {
            timePress = time;                                           // то сохраняем время её нажатия
            Keyboard::Buffer::AppendEvent(key, Action::Down);
        }
        else if (!BUTTON_IS_PRESS(state) && HappenedLongPressed())
        {
            timePress = 0;
        }
    }
}


void GovernorStruct::Process()
{
    RESET_SL(sl);

    bool stateLeft = (READ_RL(rlA) != 0);
    bool stateRight = (READ_RL(rlB) != 0);

    SET_SL(sl);

    if (stateLeft && stateRight)
    {
        prevStateIsSame = true;
    }
    else if (prevStateIsSame && stateLeft && !stateRight)
    {
        Keyboard::Buffer::AppendEvent(key, Action::RotateLeft);
        prevStateIsSame = false;
    }
    else if (prevStateIsSame && !stateLeft && stateRight)
    {
        Keyboard::Buffer::AppendEvent(key, Action::RotateRight);
        prevStateIsSame = false;
    }
}


void Keyboard::Buffer::AppendEvent(Key::E key, Action::E action)
{
    commands[pointer].key = key;
    commands[pointer].action = action;
    pointer++;
}


bool Keyboard::Buffer::IsEmpty()
{
    return (pointer == 0);
}


Control Keyboard::Buffer::GetNextControl()
{
    Control control;

    if (IsEmpty())
    {
        control.key = Key::None;
    }
    else
    {
        control = commands[0];
        for (int i = 1; i < pointer; i++)
        {
            commands[i - 1] = commands[i];
        }
        --pointer;
    }

    return control;
}
