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

// При обнаружении нажатия кнопки сюда записывается время нажатия
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
                if ((timePress[rl][sl] != 0) && (timePress[rl][sl] != UINT_MAX))    // Если клавиша находится в нажатом положении
                {
                    uint delta = time - timePress[rl][sl];
                    if(delta > 500)                                                 // Если прошло более 500 мс с момента нажатия -
                    {
                        timePress[rl][sl] = UINT_MAX;
                        AppendEvent(keys[rl][sl], Action::Long);                    // это будет длинное нажатие
                    }
                    else if (delta > 100 &&                                         // Если прошло более 100 мс с момента нажатия
                        !BUTTON_IS_PRESS(state))                                    // и сейчас кнопка находится в отжатом состоянии
                    {
                        timePress[rl][sl] = UINT_MAX;                               // То учитываем это в массиве
                        AppendEvent(keys[rl][sl], Action::Up);                      // И сохраняем отпускание кнопки в буфере команд
                    }
                }
                else if (BUTTON_IS_PRESS(state) && timePress[rl][sl] != UINT_MAX)   // Если кнопка нажата
                {
                    timePress[rl][sl] = time;                                       // то сохраняем время её нажатия
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
    // Детектируем кнопку
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

        if(press && prevPressButton && time - timePrevPress > 500)          // Если нажатие длится более 0.5 сек
        {
//            Keyboard::AppendEvent(Key::RegButton, Action::Long);                                     // посылаем длинное нажатие
            needDetectButton = false;
            prevPressButton = false;
            timePrevPress = 0;
        }

        if(timePrevPress == 0)                                              // Если кнопка находилась ранее в отжатом положении
        {
            if(press)                                                       // И теперь нажата
            {
                timePrevPress = time;
                prevPressButton = true;
//                Keyboard::AppendEvent(Key::RegButton, Action::Down);
            }
        }
        else                                                                // Ексли копка была нажата ранее
        {
            if(time - timePrevPress > 50)                                   // то проверять в следующий раз будем не ранее чем через 50 мс
            {                                                               // во избежание дребезга контактов
                if(!press)
                {
//                    Keyboard::AppendEvent(Key::RegButton, Action::Up);
                    timePrevPress = 0;
                    prevPressButton = false;
                }
            }
        }
    }


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
