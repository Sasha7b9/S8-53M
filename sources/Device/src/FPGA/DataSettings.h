#pragma once
//#include "common/Keyboard/Controls_.h"
#include "Settings/SettingsTypes.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_.h"


struct DataSettings
{
    void*         addrNext;               // ����� ��������� ������.
    void*         addrPrev;               // ����� ���������� ������.
    uint          rShiftCh0       : 10;   // �������� �� ����������
    uint          rShiftCh1       : 10;
    uint          trigLevCh0      : 10;   // ������� �������������
    int16         tShift;                 // �������� �� �������
    ModeCouple::E modeCouple1     : 2;
    Range::E      range[2];               // ������� �� ���������� ����� �������.

    uint          trigLevCh1      : 10;
    uint          length1channel  : 11;   // ������� �������� � ������ ����� ��������� ������ ������
    TBase::E      tBase           : 5;    // ������� �� �������
    ModeCouple::E modeCouple0     : 2;    // ����� ������ �� �����
    uint          peakDet         : 2;    // ������� �� ������� ��������
    uint          enableCh0       : 1;    // ������� �� ����� 0
    uint          enableCh1       : 1;    // ������� �� ����� 1

    uint          inverseCh0      : 1;
    uint          inverseCh1      : 1;
    Divider::E    multiplier0     : 1;
    Divider::E    multiplier1     : 1;
    PackedTime    time;
};
