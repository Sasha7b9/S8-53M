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


class Timer
{
public:

    static void PauseOnTicks(uint numTicks);
    // ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ��������� ����� �������� �� 1��.
    static void Update1ms();
    // ������� ���������� �� ���������� ���������� ������� ��� ������ ��������. ����������� ���������� ����� �������� �� 10��.
    static void Update10ms();
         
    static void Enable(TypeTimer type, int timeInMS, pFuncVV);
    // ���������� ������.
    static void Disable(TypeTimer type);
    // ������������� ������. ��������� ����� ����� �������� Timer_Continue().
    static void Pause(TypeTimer type);
    // ���������� ������ �������, ����� ����������������� �������� Timer_Pause().
    static void Continue(TypeTimer type);
    // � ������� ���� ������� ����� ������, �������� �� ������.
    static bool IsRun(TypeTimer type);
    // ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����.
    static void StartLogging();
    
    static uint LogPointUS(char *name);
    
    static uint LogPointMS(char *name);
};
