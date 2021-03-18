#include "defines.h"
#include "common/Log_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Debug_.h"


static uint timeStart;
static uint timePrev;
static uint timeCounter;
static uint timeStartCounter;

// ����� ������ ��������������
volatile static uint timeStartMS = 0;
// ����� ���������� ����� ��������������
volatile static uint timePrevMS = 0;

const char *Debug::file[10] = { 0 };
int   Debug::line[10] = { 0 };
int Debug::posItem = -1;


void Debug::StartProfilingMS()
{
    timeStartMS = TIME_MS;
    timePrevMS = TIME_MS; //-V656
}


uint Debug::PointProfilingMS(const char *_file, int _line)
{
    uint d = TIME_MS - timePrevMS;
    LOG_WRITE("%s %d %d", _file, _line, d);
    timePrevMS = TIME_MS;

    return d;
}


void Debug::StartProfilingUS()
{
    timeStart = gTimerTics;
    timePrev = timeStart;
}


uint Debug::PointProfilingUS(const char *_file, int _line)
{
    uint d = gTimerTics - timePrev;
    LOG_WRITE("%s:%d - %d us", _file, _line, d);
    timePrev = gTimerTics;
    return d;
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = gTimerTics;
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (gTimerTics - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
