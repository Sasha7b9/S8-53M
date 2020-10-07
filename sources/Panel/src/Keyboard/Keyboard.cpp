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

// ��� ����������� ������� ������ ���� ������������ ����� �������
static uint timePress[NUM_SL][NUM_RL];


static const Key::E keys[NUM_SL][NUM_RL] =     
//    RL0                   RL1            RL2           RL3         RL4          RL5           RL6         RL7
    {{Key::ChannelA,        Key::Count,    Key::Count,   Key::Count, Key::Count,  Key::Count,   Key::Count, Key::Menu},  // SL0
     {Key::ChannelB,        Key::Count,    Key::Count,   Key::Count, Key::Count,  Key::Count,   Key::Count, Key::F1},    // SL1
     {Key::Time,            Key::Count,    Key::Count,   Key::Count, Key::Count,  Key::Count,   Key::Count, Key::F2},    // SL2
     {Key::Synchronization, Key::Count,    Key::Count,   Key::Count, Key::Start,  Key::None,    Key::None,  Key::F3},    // SL3
     {Key::Cursors,         Key::Measures, Key::Display, Key::Help,  Key::Memory, Key::Service, Key::None,  Key::F4},    // SL4
     {Key::None,            Key::Count,    Key::Count,   Key::Count, Key::None,   Key::None,    Key::None,  Key::F5}};   // SL5

static Pin *sls[] = { &pinSL0, &pinSL1, &pinSL2, &pinSL3, &pinSL4, &pinSL5 };
static Pin *rls[] = { &pinRL0, &pinRL1, &pinRL2, &pinRL3, &pinRL4, &pinRL5, &pinRL6, &pinRL7 };

#define SET_ALL_SL      pinSL0.Set(); pinSL1.Set(); pinSL2.Set(); pinSL3.Set(); pinSL4.Set(); pinSL5.Set()
#define SET_SL(n)       sls[n]->Set();
#define RESET_SL(n)     sls[n]->Reset();
#define READ_RL(n)      rls[n]->Read();


static bool init = false;


void Keyboard::Init()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            timePress[i][j] = 0;
        }
    }

    pointer = 0;

    HAL_TIM2::Init(&Keyboard::Update);

    init = true;
}


void Keyboard::Update()
{
    if(!init)
    {
        return;
    }
    
    uint time = TIME_MS;

    for (int sl = 0; sl < NUM_SL; sl++)
    {
        RESET_SL(sl);

        for (int rl = 0; rl < NUM_RL; rl++)
        {
            uint state = READ_RL(rl);

            Key::E key = keys[rl][sl];

            if (key != Key::None)
            {
                if ((timePress[rl][sl] != 0) && (timePress[rl][sl] != UINT_MAX))    // ���� ������� ��������� � ������� ���������
                {
                    uint delta = time - timePress[rl][sl];
                    if(delta > 500)                                                 // ���� ������ ����� 500 �� � ������� ������� -
                    {
                        timePress[rl][sl] = UINT_MAX;
                        AppendEvent(keys[rl][sl], Action::Long);                    // ��� ����� ������� �������
                    }
                    else if (delta > 100 &&                                         // ���� ������ ����� 100 �� � ������� �������
                        !BUTTON_IS_PRESS(state))                                    // � ������ ������ ��������� � ������� ���������
                    {
                        timePress[rl][sl] = UINT_MAX;                               // �� ��������� ��� � �������
                        AppendEvent(keys[rl][sl], Action::Up);                      // � ��������� ���������� ������ � ������ ������
                    }
                }
                else if (BUTTON_IS_PRESS(state) && timePress[rl][sl] != UINT_MAX)   // ���� ������ ������
                {
                    timePress[rl][sl] = time;                                       // �� ��������� ����� � �������
                    AppendEvent(keys[rl][sl], Action::Down);
                }
                else if(!BUTTON_IS_PRESS(state) && timePress[rl][sl] == UINT_MAX)
                {
                    timePress[rl][sl] = 0;
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
    
    bool press = false; // HAL_PIO::ReadPin('C', HPin::_2) ? false : true;

    if(!press)
    {
        needDetectButton = true;
    }

    if(needDetectButton)
    {
        uint time = TIME_MS;

        if(press && prevPressButton && time - timePrevPress > 500)          // ���� ������� ������ ����� 0.5 ���
        {
//            Keyboard::AppendEvent(Key::RegButton, Action::Long);                                     // �������� ������� �������
            needDetectButton = false;
            prevPressButton = false;
            timePrevPress = 0;
        }

        if(timePrevPress == 0)                                              // ���� ������ ���������� ����� � ������� ���������
        {
            if(press)                                                       // � ������ ������
            {
                timePrevPress = time;
                prevPressButton = true;
//                Keyboard::AppendEvent(Key::RegButton, Action::Down);
            }
        }
        else                                                                // ����� ����� ���� ������ �����
        {
            if(time - timePrevPress > 50)                                   // �� ��������� � ��������� ��� ����� �� ����� ��� ����� 50 ��
            {                                                               // �� ��������� �������� ���������
                if(!press)
                {
//                    Keyboard::AppendEvent(Key::RegButton, Action::Up);
                    timePrevPress = 0;
                    prevPressButton = false;
                }
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


void Keyboard::AppendEvent(Key::E key, Action::E action)
{
    commands[pointer].key = key;
    commands[pointer].action = action;
    pointer++;
}


bool Keyboard::BufferIsEmpty()
{
    return pointer == 0;
}


Control Keyboard::GetNextControl()
{
    Control control;

    if (BufferIsEmpty())
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
