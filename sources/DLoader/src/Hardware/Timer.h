#pragma once 
#include "defines.h"
#include "Utils/GlobalFunctions.h"


struct TypeTimer { enum E
{
    PressKey,                   // ����� �������������, ����� �������� ������� �������.
    ShowLevelRShift0,           // ����� �������������, ����� ���������� ��������� ����� �������� ������ 1.
    ShowLevelRShift1,           // ����� �������������, ����� ���������� ��������� ����� �������� ������ 2.
    ShowLevelTrigLev,           // ����� �������������, ����� ���������� ��������� ����� ������ �������������
    NumSignalsInSec,            // ��� ��������� ���������� ���������� ������� � �������.
    FlashDisplay,               // ������ ��� ��������� �������� ������ �����-������ �������.
    P2P,                        // ������ ��� ������ ������ � ������ ����������� ������.
    ShowMessages,               // ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
    MenuAutoHide,               // ������ ��� ������� ������� ��������� ����.
    RShiftMarkersAutoHide,      // ������ ��� ������� ������� ��������� �������������� ������� �����.
    Temp,                       // ���������������, ��� ����� ����.
    StopSound,                  // ��������� ����
    TemporaryPauseFPGA,         // ��������� ����� ��� �������� ������� �� ������ ����� �������� �����
    TimerDrawHandFunction,      // ���� ������ ����� ������������ �������, ������������ ���� ��������� ���������
    Count                       // ����� ���������� ��������.
};};


// ���������������� ���������� ��������� �� timeMS �����������
void Timer_PauseOnTime(uint timeMS);
// ���������������� ���������� ��������� �� numTicks �����
void Timer_PauseOnTicks(uint numTicks);
// ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ��������� ����� �������� �� 1��.
void Timer_Update1ms(void);
// ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ���������� ����� �������� �� 10��.
void Timer_Update10ms(void);
// ������ �������
void Timer_Enable(TypeTimer::E type,   // ��� �������
                  int timeInMS,     // ����� ������������ �������
                  pFuncVV func      // �������, ������� ���������� ������ ���, ����� ����������� ������
                  );
// ���������� ������
void Timer_Disable(TypeTimer::E type);
// ������������� ������. ��������� ����� ����� �������� Timer_Continue()
void Timer_Pause(TypeTimer::E type);
// ���������� ������ �������, ����� ����������������� �������� Timer_Pause()
void Timer_Continue(TypeTimer::E type);
// � ������� ���� ������� ����� ������, �������� �� ������
bool Timer_IsRun(TypeTimer::E type);
// ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����.
void Timer_StartLogging(void);               

uint Timer_LogPointUS(char *name);

uint Timer_LogPointMS(char *name);

struct Timer
{
    static void Update1ms();
};
