// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"


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
    LOG_WRITE("%s %.2f us", name, (float)(interval) / 120.0F);
    return interval;
}


uint Timer::LogPointMS(char *name)
{
    uint interval = gTimerTics - timePrevPoint;
    timePrevPoint = gTimerTics;
    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}


void Timer::Enable(TypeTimer::E type, int timeInMS, pFuncVV func)
{
    f[type] = func;
    reactionTimeMS[type] = timeInMS;
    currentTimeMS[type] = timeInMS - 1;
    isRun[type] = true;
}


void Timer::Disable(TypeTimer::E type)
{
    isRun[type] = false;
}


void Timer::Update1ms()
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
