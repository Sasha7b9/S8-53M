#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"


struct DataSettings
{
    DataSettings() : enabled_a(0), enabled_b(0), is_clean(1) {} //-V730

    uint                enabled_a    : 1;   // ������� �� ����� 0
    uint                enabled_b    : 1;   // ������� �� ����� 1
    uint                r_shift_a    : 10;  // �������� �� ����������
    uint                r_shift_b    : 10;
    uint                trig_lev_a   : 10;  // ������� �������������
    uint                trig_lev_b   : 10;
    uint                couple_a     : 2;   // ����� ������ �� �����
    uint                couple_b     : 2;
    uint                tbase        : 5;   // ������� �� �������
    uint                peak_det     : 2;   // ������� �� ������� ��������
    Divider::E          multiplier_a : 1;
    Divider::E          multiplier_b : 1;
    EnumPointsFPGA::E enum_points  : 4;   // enum ����� � ������
    uint                is_clean     : 1;   // ������������� � 1 �������� ��������, ��� ������ ��� (��������� ������
                                            // ��� �������)
    PackedTime          time;
    int16               tshift;             // �������� �� �������
    Range::E            range[2];           // ������� �� ���������� ����� �������.


    void Fill();

    TBase::E GetTBase() const;

    // ���������� ���������� ����, ���������� ����� �������
    uint BytesInChannel() const;

    // ���������� ���������� ����, ���������� ������ �������� (� ������ �� ���������)
    uint BytesInData() const;

    // ������� ����� � ����� ������
    uint PointsInChannel() const;

    bool IsEnabled(const Channel &ch) const;

    bool IsEnabledPeakDet() const;
};
