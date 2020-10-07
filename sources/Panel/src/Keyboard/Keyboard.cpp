#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include <limits>


static Control commands[10];
static int pointer = 0;


static void DetectRegulator();

#define NUM_SL 6
#define NUM_RL 8    

#define BUTTON_IS_PRESS(state)  ((state) == 0)

#define KEY(sl, rl) (keys[sl][rl])


struct KeyStruct
{
    KeyStruct(Key::E k) : key(k), timePress(0U) { }
    // ���������� true, ���� ����� �������� � ������� ���������
    bool IsPressed() const { return (timePress != 0); }
    // ���������� true, ���� ��������� ������� �������
    bool HappenedLongPressed() const { return (timePress == UINT_MAX); }

    bool IsValid() const { return (key != Key::None) && (key != Key::Count); }
    Key::E key;
    uint   timePress;   // ����� ������� ������
};


static KeyStruct keys[NUM_SL][NUM_RL] =     
//    RL0                   RL1            RL2           RL3         RL4          RL5           RL6         RL7
    {{Key::ChannelA,        Key::Count,    Key::Count,   Key::Count, Key::Count,  Key::Count,   Key::Count, Key::Menu},  // SL0
     {Key::ChannelB,        Key::Count,    Key::Count,   Key::Count, Key::Count,  Key::Count,   Key::Count, Key::F1},    // SL1
     {Key::Time,            Key::Count,    Key::Count,   Key::Count, Key::Count,  Key::Count,   Key::Count, Key::F2},    // SL2
     {Key::Synchronization, Key::Count,    Key::Count,   Key::Count, Key::Start,  Key::None,    Key::None,  Key::F3},    // SL3
     {Key::Cursors,         Key::Measures, Key::Display, Key::Help,  Key::Memory, Key::Service, Key::None,  Key::F4},    // SL4
     {Key::None,            Key::Count,    Key::Count,   Key::Count, Key::None,   Key::None,    Key::None,  Key::F5}};   // SL5

static Pin *sls[NUM_SL] = { &pinSL0, &pinSL1, &pinSL2, &pinSL3, &pinSL4, &pinSL5 };
static Pin *rls[NUM_RL] = { &pinRL0, &pinRL1, &pinRL2, &pinRL3, &pinRL4, &pinRL5, &pinRL6, &pinRL7 };

#define SET_ALL_SL      pinSL0.Set(); pinSL1.Set(); pinSL2.Set(); pinSL3.Set(); pinSL4.Set(); pinSL5.Set()
#define SET_SL(n)       sls[n]->Set();
#define RESET_SL(n)     sls[n]->Reset();
#define READ_RL(n)      rls[n]->Read();


void Keyboard::Init()
{
    pointer = 0;

    HAL_TIM2::Init(&Keyboard::Update);
}


void Keyboard::Update()
{
    uint time = TIME_MS;

    for (int sl = 0; sl < NUM_SL; sl++)
    {
        RESET_SL(sl);

        for (int rl = 0; rl < NUM_RL; rl++)
        {
            uint state = READ_RL(rl);

            KeyStruct &key = KEY(sl, rl);

            if (key.IsValid())
            {
                if (key.IsPressed() && !key.HappenedLongPressed())                  // ���� ������� ��������� � ������� ���������
                {
                    uint delta = time - key.timePress;
                    if(delta > 500)                                                 // ���� ������ ����� 500 �� � ������� ������� -
                    {
                        key.timePress = UINT_MAX;
                        Buffer::AppendEvent(key.key, Action::Long);                 // ��� ����� ������� �������
                    }
                    else if (delta > 100 &&                                         // ���� ������ ����� 100 �� � ������� �������
                        !BUTTON_IS_PRESS(state))                                    // � ������ ������ ��������� � ������� ���������
                    {
                        key.timePress = UINT_MAX;                                   // �� ��������� ��� � �������
                        Buffer::AppendEvent(key.key, Action::Up);                   // � ��������� ���������� ������ � ������ ������
                    }
                }
                else if (BUTTON_IS_PRESS(state) && !key.HappenedLongPressed())      // ���� ������ ������
                {
                    key.timePress = time;                                           // �� ��������� ����� � �������
                    Buffer::AppendEvent(key.key, Action::Down);
                }
                else if(!BUTTON_IS_PRESS(state) && key.HappenedLongPressed())
                {
                    key.timePress = 0;
                }
            }
        }
        
        SET_ALL_SL;
    }
    
    DetectRegulator();

    SET_ALL_SL;
}


static void DetectRegulator()
{
    // ����������� ������
    static bool prevPressButton = false;
    static uint timePrevPress = 0;
    static bool needDetectButton = true;
    
    needDetectButton = true;

    if(needDetectButton)
    {
        uint time = TIME_MS;

        if(prevPressButton && time - timePrevPress > 500)          // ���� ������� ������ ����� 0.5 ���
        {
            needDetectButton = false;
            prevPressButton = false;
            timePrevPress = 0;
        }

        if(timePrevPress == 0)                                              // ���� ������ ���������� ����� � ������� ���������
        {
            timePrevPress = time;
            prevPressButton = true;
        }
        else                                                                // ����� ����� ���� ������ �����
        {
            if(time - timePrevPress > 50)                                   // �� ��������� � ��������� ��� ����� �� ����� ��� ����� 50 ��
            {                                                               // �� ��������� �������� ���������
                timePrevPress = 0;
                prevPressButton = false;
            }
        }
    }


    // ����������� �������
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
