#pragma once 
#include "defines.h"
#include "Utils/GlobalFunctions.h"


typedef enum
{
    kPressKey,                  ///< ����� �������������, ����� �������� ������� �������.
    kShowLevelRShift0,          ///< ����� �������������, ����� ���������� ��������� ����� �������� ������ 1.
    kShowLevelRShift1,          ///< ����� �������������, ����� ���������� ��������� ����� �������� ������ 2.
    kShowLevelTrigLev,          ///< ����� �������������, ����� ���������� ��������� ����� ������ �������������
    kNumSignalsInSec,           ///< ��� ��������� ���������� ���������� ������� � �������.
    kFlashDisplay,              ///< ������ ��� ��������� �������� ������ �����-������ �������.
    kP2P,                       ///< ������ ��� ������ ������ � ������ ����������� ������.
    kShowMessages,              ///< ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
    kMenuAutoHide,              ///< ������ ��� ������� ������� ��������� ����.
    kRShiftMarkersAutoHide,     ///< ������ ��� ������� ������� ��������� �������������� ������� �����.
    kTemp,                      ///< ���������������, ��� ����� ����.
    kStopSound,                 ///< ��������� ����
    kTemporaryPauseFPGA,        ///< ��������� ����� ��� �������� ������� �� ������ ����� �������� �����
    kTimerDrawHandFunction,     ///< ���� ������ ����� ������������ �������, ������������ ���� ��������� ���������
    TypeTimerSize               ///< ����� ���������� ��������.
} TypeTimer;


// ���������������� ���������� ��������� �� timeMS �����������
void Timer_PauseOnTime(uint timeMS);
// ���������������� ���������� ��������� �� numTicks �����
void Timer_PauseOnTicks(uint numTicks);
// ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ��������� ����� �������� �� 1��.
void Timer_Update1ms(void);
// ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ���������� ����� �������� �� 10��.
void Timer_Update10ms(void);
// ������ �������
void Timer_Enable(TypeTimer type,   ///< ��� �������
                  int timeInMS,     ///< ����� ������������ �������
                  pFuncVV func      ///< �������, ������� ���������� ������ ���, ����� ����������� ������
                  );
// ���������� ������
void Timer_Disable(TypeTimer type);
// ������������� ������. ��������� ����� ����� �������� Timer_Continue()
void Timer_Pause(TypeTimer type);
// ���������� ������ �������, ����� ����������������� �������� Timer_Pause()
void Timer_Continue(TypeTimer type);
// � ������� ���� ������� ����� ������, �������� �� ������
bool Timer_IsRun(TypeTimer type);
// ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����.
void Timer_StartLogging(void);               

uint Timer_LogPointUS(char *name);

uint Timer_LogPointMS(char *name);

struct Timer
{
    static void Update1ms();
};
