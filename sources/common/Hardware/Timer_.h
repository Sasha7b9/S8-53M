// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 


struct TypeTimer { enum E
{
    ShowLevelRShiftA,         // ����� �������������, ����� ���������� ��������� ����� �������� ������ 1.
    ShowLevelRShiftB,         // ����� �������������, ����� ���������� ��������� ����� �������� ������ 2.
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
         
    static void Enable(TypeTimer::E type, int timeInMS, pFuncVV);
    // ���������� ������.
    static void Disable(TypeTimer::E type);
    // ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����.
    static void StartLogging();
    
    static uint LogPointUS(char *name);
    
    static uint LogPointMS(char *name);
};
