#pragma once
#include "SettingsTypes.h"
#include "defines.h"


#define SET_TBASE                           (set.time.tBase)        // SettingTime.tBase

#define TSHIFT                              (set.time.tShiftRel)    // SettingsTime.tShiftRel

#define TIME_DIV_XPOS                       (set.time.timeDivXPos)  // SettingsTime.timeDivXPos
#define TIME_DIV_XPOS_IS_SHIFT_IN_MEMORY    (TIME_DIV_XPOS == FunctionTime::ShiftInMemory)

#define SET_TPOS                            (set.time.tPos)         // SettingsTime.tPos
#define SET_TPOS_IS_LEFT                    (SET_TPOS == TPos::Left)
#define SET_TPOS_IS_CENTER                  (SET_TPOS == TPos::Center)
#define SET_TPOS_IS_RIGHT                   (SET_TPOS == TPos::Right)

#define SAMPLE_TYPE                         (set.time.sampleType)   // SettingsTime.sampleType
#define SAMPLE_TYPE_IS_EQUAL                (SAMPLE_TYPE == SampleType::Equal)
#define SAMPLE_TYPE_IS_REAL                 (SAMPLE_TYPE == SampleType::Real)

#define PEAKDET                             (set.time.peakDet)      // SettingsTime.peakDet
#define PEAKDET_IS_DISABLE                  (PEAKDET == PeackDetMode::Disable)
#define PEAKDET_IS_ENABLE                   (PEAKDET == PeackDetMode::Enable)

#define SET_SELFRECORDER                    (set.time.selfRecorder) // SettingsTime.selfRecorder


// ��������� ��� X.
struct SettingsTime
{ //-V802
    TBase::E            tBase;          // ������� �� �������.
    int16               tShiftRel;      // �������� �� �������.
    FunctionTime::E     timeDivXPos;
    TPos::E             tPos;           // �������� ������������� � ������.
    SampleType::E       sampleType;     // ��� ������� ��� ������ �������������.
    PeackDetMode::E     peakDet;        // ����� ������ �������� ���������
    bool                selfRecorder;   // ������� �� ����� ���������.
    ENUM_POINTS_FPGA::E oldNumPoints;   // \brief ����� ������������� � ����� �������� ���������, ������������� ���������� ����� � 1024, � ���� 
                                        // ���������� ��, ��� ����, ����� ����� ������������.
};


// ��������� ������� �� �������.
void sTime_SetTBase(TBase::E tBase);
// ��������� �������� �� ������� � ������������� ��������.
void sTime_SetTShift(int16 shift);                   
// ������ �������� ���������� ������ ������������� � ������.
int sTime_TPosInPoints(PeackDetMode::E peakDet, int numPoints, TPos::E tPos);
// �������� �� ������� � ������ ������. �.�. �� ����� � ��� �������� ��� ��� ���������, ��� �������� ���������� �� ����������� � ���������� ����� � ��� ����.
int sTime_TShiftInPoints(PeackDetMode::E peakDet);
// ����������� �������� �� �������, ������� ����� ���� �������� � ���������� �����.
int16 sTime_TShiftMin();
// �������� �� �������, ��������������� ������� TPos.
int16 sTime_TShiftZero();
// ���� true, ������, �������� ������������.
bool sTime_RandomizeModeEnabled();
