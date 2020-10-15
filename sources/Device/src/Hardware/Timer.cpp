#include "defines.h"
#include "common/Log_c.h"
#include "Hardware/Timer.h"



static void (*f[TypeTimer::Count])() = {0};
static int reactionTimeMS[TypeTimer::Count] = {0};
static int currentTimeMS[TypeTimer::Count] = {0};
static bool isRun[TypeTimer::Count] = {false};



void Timer::PauseOnTicks(uint numTicks)
{
    uint startTicks = gTimerTics;
    while (gTimerTics - startTicks < numTicks) {};
}


static uint timeStartLogging = 0;
static uint timePrevPoint = 0;


void Timer::StartLogging()
{
    timeStartLogging = gTimerTics;
    timePrevPoint = timeStartLogging;
}


uint Timer::LogPointUS(char *name)
{
    uint interval = gTimerTics - timePrevPoint;
    timePrevPoint = gTimerTics;
    LOG_WRITE("%s %.2f us", name, static_cast<float>(interval) / 120.0F);
    return interval;
}


uint Timer::LogPointMS(char *name)
{
    uint interval = gTimerTics - timePrevPoint;
    timePrevPoint = gTimerTics;
    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}


void Timer::Enable(TypeTimer::E type, int timeInMS, void(*eF)())
{
    f[type] = eF;
    reactionTimeMS[type] = timeInMS;
    currentTimeMS[type] = timeInMS - 1;
    isRun[type] = true;
}


void Timer::Disable(TypeTimer::E type)
{
    isRun[type] = false;
}


void Timer_Pause(TypeTimer::E type)
{
    isRun[type] = false;
}


void Timer_Continue(TypeTimer::E type)
{
    isRun[type] = true;
}


bool Timer_IsRun(TypeTimer::E type)
{
    return isRun[type];
};


void Timer::Update1ms(void)
{
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


void Timer_Update10ms(void)
{
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
