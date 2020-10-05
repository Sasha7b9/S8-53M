#pragma once
#include "Settings/SettingsDisplay.h"


enum Warning
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
    Warning_NumWarnings
};

enum WarningWithNumber
{
    ExcessValues        // ���������� �������� ���������� �������� � "����������", "����������", "��� ����"
};


ENumSignalsInSec::E Tables_NumSignalsInSecToENUM(int enumSignalsInSec);

int Tables_ENUMtoNumSignalsInSec(ENumSignalsInSec::E enumSignalsInSec);

const char* Tables_GetWarning(Warning warning);

const char* Tables_GetTBaseString(TBase::E tBase);

const char* Tables_GetTBaseStringEN(TBase::E tBase);

const char* Tables_RangeNameFromValue(Range range);
// ���������� ����� ������ �� 1 �� 2
int Tables_GetNumChannel(Channel::E chan);

extern const char* symbolsAlphaBet[0x48];
// ������� ������ �� ������� symbolsAlphaBet
void DrawStr(int index, int x, int y);
