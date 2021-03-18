#pragma once
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_.h"


struct DataSettings
{
    uint                enabled_a : 1;    // ������� �� ����� 0
    uint                enabled_b : 1;    // ������� �� ����� 1
    uint                rShiftCh0       : 10;   // �������� �� ����������
    uint                rShiftCh1       : 10;
    uint                trigLevCh0 : 10;   // ������� �������������
    uint                trigLevCh1 : 10;
    ModeCouple::E       modeCouple0 : 2;    // ����� ������ �� �����
    ModeCouple::E       modeCouple1 : 2;
    TBase::E            tBase : 5;    // ������� �� �������
    int16               tShift;                 // �������� �� �������
    Range::E            range[2];               // ������� �� ���������� ����� �������.
    uint                peakDet : 2;    // ������� �� ������� ��������
    Divider::E          multiplier0 : 1;
    Divider::E          multiplier1 : 1;
    ENUM_POINTS_FPGA::E enum_points     : 4;    // enum ����� � ������
    PackedTime          time;

    void Fill();

    uint BytesInChannel() const;

    uint PointsInChannel() const;

    bool IsEnabled(const Channel &ch) const;

    bool IsEnabledPeakDet() const;
};
