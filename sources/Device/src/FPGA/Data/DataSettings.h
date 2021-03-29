#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"


struct DataSettings
{
    DataSettings() : enabled_a(0), enabled_b(0) {} //-V730

    uint                enabled_a   : 1;    // ������� �� ����� 0
    uint                enabled_b   : 1;    // ������� �� ����� 1
    uint                r_shift_a   : 10;   // �������� �� ����������
    uint                r_shift_b   : 10;
    uint                trig_lev_a  : 10;   // ������� �������������
    uint                trig_lev_b  : 10;
    ModeCouple::E       couple_a    : 2;    // ����� ������ �� �����
    ModeCouple::E       couple_b    : 2;
    TBase::E            tbase       : 5;    // ������� �� �������
    int16               tShift;             // �������� �� �������
    Range::E            range[2];           // ������� �� ���������� ����� �������.
    uint                peakDet     : 2;    // ������� �� ������� ��������
    Divider::E          multiplier0 : 1;
    Divider::E          multiplier1 : 1;
    ENUM_POINTS_FPGA::E enum_points : 4;    // enum ����� � ������
    PackedTime          time;

    void Fill();

    // ���������� ���������� ����, ���������� ����� �������
    uint BytesInChannel() const;

    // ���������� ���������� ����, ���������� ������ �������� (� ������ �� ���������)
    uint BytesInData() const;

    // ������� ����� � ����� ������
    uint PointsInChannel() const;

    bool IsEnabled(const Channel &ch) const;

    bool IsEnabledPeakDet() const;
};
