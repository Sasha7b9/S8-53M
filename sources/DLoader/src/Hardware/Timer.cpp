#include "defines.h"
#include "Timer.h"
#include "common/Hardware/HAL/HAL_c.h"


static volatile uint timerMS = 0;
static void (*f[TypeTimer::Count])() = {0};
static int reactionTimeMS[TypeTimer::Count] = {0};
static int currentTimeMS[TypeTimer::Count] = {0};
static bool isRun[TypeTimer::Count] = {false};

static uint timeStartLogging = 0;
static uint timePrevPoint = 0;



void Timer_PauseOnTicks(uint numTicks)
{
    uint startTicks = gTimerTics;
    while (gTimerTics - startTicks < numTicks) {};
}


void Timer_PauseOnTime(uint timeMS)
{
    uint time = timerMS;
    while(timerMS - time < timeMS) {};
}


void Timer_StartLogging()
{
    timeStartLogging = gTimerTics;
    timePrevPoint = timeStartLogging;
}


uint Timer_LogPointUS(char *)
{
    uint interval = gTimerTics - timePrevPoint;
    timePrevPoint = gTimerTics;
    return interval;
}


uint Timer_LogPointMS(char *)
{
    uint interval = gTimerMS - timePrevPoint;
    timePrevPoint = gTimerMS;
    return interval;
}


void Timer_Enable(TypeTimer::E type, int timeInMS, pFuncVV eF)
{
    f[type] = eF;
    reactionTimeMS[type] = timeInMS;
    currentTimeMS[type] = timeInMS - 1;
    isRun[type] = true;
}


void Timer_Disable(TypeTimer::E type)
{
    isRun[type] = false;
}


void Timer_Pause(TypeTimer::E type)
{
    Timer_Disable(type);
}


void Timer_Continue(TypeTimer::E type)
{
    isRun[type] = true;
}


bool Timer_IsRun(TypeTimer::E type)
{
    return isRun[type];
};


void Timer::Update1ms()
{
    timerMS++;
    for(int num = 0; num < TypeTimer::Count; num++)
    {
        if(isRun[num])
        {
            currentTimeMS[num]--;
            if(currentTimeMS[num] < 0)
            {
                f[num]();
                currentTimeMS[num] = reactionTimeMS[num] - 1;
            }
            
        }
    }
}


void Timer_Update10ms()
{
    timerMS += 10;
    for(int num = 0; num < TypeTimer::Count; num++)
    {
        if(isRun[num])
        {
            currentTimeMS[num] -= 10;
            if(currentTimeMS[num] < 0)
            {
                f[num]();
                currentTimeMS[num] = reactionTimeMS[num] - 1;
            }

        }
    }
}
