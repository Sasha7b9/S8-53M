#pragma once 
#include "defines.h"
#include "Utils/GlobalFunctions.h"


struct TypeTimer { enum E
{
    PressKey,                 // ����� �������������, ����� �������� ������� �������.
    ShowLevelRShift0,         // ����� �������������, ����� ���������� ��������� ����� �������� ������ 1.
    ShowLevelRShift1,         // ����� �������������, ����� ���������� ��������� ����� �������� ������ 2.
    ShowLevelTrigLev,         // ����� �������������, ����� ���������� ��������� ����� ������ �������������
    NumSignalsInSec,          // ��� ��������� ���������� ���������� ������� � �������.
    FlashDisplay,             // ������ ��� ��������� �������� ������ �����-������ �������.
    P2P,                      // ������ ��� ������ ������ � ������ ����������� ������.
    ShowMessages,             // ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
    MenuAutoHide,             // ������ ��� ������� ������� ��������� ����.
    RShiftMarkersAutoHide,    // ������ ��� ������� ������� ��������� �������������� ������� �����.
    Temp,                     // ���������������, ��� ����� ����.
    StopSound,                // ��������� ����
    TemporaryPauseFPGA,       // ��������� ����� ��� �������� ������� �� ������ ����� �������� �����
    TimerDrawHandFunction,    // ���� ������ ����� ������������ �������, ������������ ���� ��������� ���������
    Count                     // ����� ���������� ��������.
};};


class Timer
{
public:

    static void PauseOnTicks(uint numTicks);
    // ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ��������� ����� �������� �� 1��.
    static void Update1ms();
    // ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ���������� ����� �������� �� 10��.
    static void Update10ms();
         
    static void Enable(TypeTimer::E type, int timeInMS, pFuncVV);
    // ���������� ������.
    static void Disable(TypeTimer::E type);
    // ������������� ������. ��������� ����� ����� �������� Timer_Continue().
    static void Pause(TypeTimer::E type);
    // ���������� ������ �������, ����� ����������������� �������� Timer_Pause().
    static void Continue(TypeTimer::E type);
    // � ������� ���� ������� ����� ������, �������� �� ������.
    static bool IsRun(TypeTimer::E type);
    // ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����.
    static void StartLogging();
    
    static uint LogPointUS(char *name);
    
    static uint LogPointMS(char *name);
};
