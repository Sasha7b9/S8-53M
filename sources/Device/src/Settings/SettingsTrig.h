#pragma once
#include "common/Settings/SettingsTypes_.h"


#define MODE_LONG_PRESS_TRIG    (set.trig.modeLongPressTrig)            // SettingsTrig.modeLongPressTrig
// ���� \c true, �� ���������� ������� ������ \b ����� ���������� ������� ������������� � 0.
#define MODE_LONG_PRESS_TRIG_IS_LEVEL0  (MODE_LONG_PRESS_TRIG == ModeLongPressTrig::Level0)

#define TRIG_MODE_FIND          (set.trig.modeFind)                     // SettingsTrig.modeFind
#define TRIG_MODE_FIND_IS_AUTO  (TRIG_MODE_FIND == TrigModeFind::Auto)  // \c true, ���� �������������� ��������� �������������.
#define TRIG_MODE_FIND_IS_HAND  (TRIG_MODE_FIND == TrigModeFind::Hand)  // \c true, ���� ������������� ��������������� �������.


// ��������� �������������
struct SettingsTrig
{
    StartMode::E         startMode;          // ����� �������.
    TrigSource::E        source;             // ��������.
    TrigPolarity::E      polarity;           // ��� �������������.
    TrigInput::E         input;              // ���� �������������.
    int16                levelRel[3];        // ������� ������������� ��� ��� ����������.
    ModeLongPressTrig::E modeLongPressTrig;  // ����� ������ ����������� ������� ������ �����.
    TrigModeFind::E      modeFind;           // ����� ������������� - ������� ��� �������������.
};
