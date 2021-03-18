#pragma once
#include "Settings/SettingsDisplay.h"


struct Warning { enum E
{
    LimitChan1_Volts,
    LimitChan2_Volts,
    LimitSweep_Time,
    EnabledPeakDet,
    LimitChan1_RShift,
    LimitChan2_RShift,
    LimitSweep_Level,
    LimitSweep_TShift,
    TooSmallSweepForPeakDet,
    TooFastScanForSelfRecorder,
    FileIsSaved,
    SignalIsSaved,
    SignalIsDeleted,
    MenuDebugEnabled,
    MovingData,
    TimeNotSet,
    SignalNotFound,
    SetTPosToLeft1,
    SetTPosToLeft2,
    SetTPosToLeft3,
    VerySmallMeasures,
    NeedRebootDevice1,
    NeedRebootDevice2,
    ImpossibleEnableMathFunction,
    ImpossibleEnableFFT,
    FirmwareSaved,
    FullyCompletedOTP,
    Count
};};

struct WarningWithNumber { enum E
{
    ExcessValues        // ���������� �������� ���������� �������� � "����������", "����������", "��� ����"
};};


struct Tables
{
    static ENumSignalsInSec::E NumSignalsInSecToENUM(int enumSignalsInSec);
    static int ENUMtoNumSignalsInSec(ENumSignalsInSec::E enumSignalsInSec);
    static pchar GetWarning(Warning::E warning);
    static pchar RangeNameFromValue(Range::E range);
    // ���������� ����� ������ �� 1 �� 2
    static int GetNumChannel(Channel::E ch);
    // ������� ������ �� ������� symbolsAlphaBet
    static void DrawStr(int index, int x, int y);
    static pchar symbolsAlphaBet[0x48];
};
