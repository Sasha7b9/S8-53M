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

#define SET_ALL_SL      pinSL0.Set(); pinSL1.Set(); pinSL2.Set(); pinSL3.Set(); pinSL4.Set(); pinSL5.Set()
#define SET_SL(n)       sls[n]->Set();
#define RESET_SL(n)     sls[n]->Reset();
#define READ_RL(n)      rls[n]->Read();


struct GovernorStruct
{
//    GovernorStruct(Key::E k, Pin &rlA_, Pin &rlB_, Pin &sl_) : key(k), rlA(rlA_), rlB(rlB_), sl(sl_) { }
    Key::E key;
    Pin &rlA;
    Pin &rlB;
    Pin &sl;
};

#define NUM_GOVERNORS 8

static GovernorStruct governors[NUM_GOVERNORS] =
{
    {Key::RangeA,  pinRL1, pinRL2, pinSL0},
    {Key::RangeB,  pinRL1, pinRL2, pinSL1},
    {Key::RShiftA, pinRL4, pinRL5, pinSL0},
    {Key::RShiftB, pinRL4, pinRL5, pinSL1},
    {Key::TBase,   pinRL1, pinRL2, pinSL2},
    {Key::TShift,  pinRL4, pinRL5, pinSL2},
    {Key::TrigLev, pinRL1, pinRL2, pinSL3},
    {Key::Setting, pinRL1, pinRL2, pinSL5}
};


void Keyboard::Init()
{
    pointer = 0;

    HAL_TIM2::Init(&Keyboard::Update);
}


void Keyboard::Update()
{
    for (int sl = 0; sl < NUM_SL; sl++)
    {
        RESET_SL(sl);

        for (int rl = 0; rl < NUM_RL; rl++)
        {
            ProcessKey(sl, rl);
        }
        
        SET_ALL_SL;
    }
    
    for (int i = 0; i < NUM_GOVERNORS; i++)
    {
        ProcessGovernor(i);
    }

    SET_ALL_SL;
}


void Keyboard::ProcessKey(int sl, int rl)
{
    uint time = TIME_MS;

    uint state = READ_RL(rl);

    KeyStruct &key = KEY(sl, rl);

    if (key.IsValid())
    {
        if (key.IsPressed() && !key.HappenedLongPressed())                  // Если клавиша находится в нажатом положении
        {
            uint delta = time - key.timePress;
            if (delta > 500)                                                 // Если прошло более 500 мс с момента нажатия -
            {
                key.timePress = UINT_MAX;
                Buffer::AppendEvent(key.key, Action::Long);                 // это будет длинное нажатие
            }
            else if (delta > 100 &&                                         // Если прошло более 100 мс с момента нажатия
                     !BUTTON_IS_PRESS(state))                                    // и сейчас кнопка находится в отжатом состоянии
            {
                key.timePress = UINT_MAX;                                   // То учитываем это в массиве
                Buffer::AppendEvent(key.key, Action::Up);                   // И сохраняем отпускание кнопки в буфере команд
            }
        }
        else if (BUTTON_IS_PRESS(state) && !key.HappenedLongPressed())      // Если кнопка нажата
        {
            key.timePress = time;                                           // то сохраняем время её нажатия
            Buffer::AppendEvent(key.key, Action::Down);
        }
        else if (!BUTTON_IS_PRESS(state) && key.HappenedLongPressed())
        {
            key.timePress = 0;
        }
    }
}


void Keyboard::ProcessGovernor(int i)
{
    GovernorStruct governor = governors[i];
    governor.key = Key::None;
}


static void DetectRegulator()
{
    // Детектируем поворот
    static bool prevStatesIsOne = false;

    bool stateLeft = false; // HAL_PIO::ReadPin('C', HPin::_0);
    bool stateRight = false; // HAL_PIO::ReadPin('C', HPin::_1);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
//        Keyboard::AppendEvent(Key::RotateLeft, Action::Down);
        prevStatesIsOne = false;
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
//        Keyboard::AppendEvent(Key::RotateRight, Action::Down);
        prevStatesIsOne = false;
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
