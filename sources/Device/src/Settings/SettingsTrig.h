#pragma once
#include "common/Settings/SettingsTypes_.h"


#define TRIG_POLARITY           (set.trig.polarity)                     // SettingsTrig.polarity
#define TRIG_POLARITY_IS_FRONT  (TRIG_POLARITY == TrigPolarity::Front)  // \c true, ���� ������������� �� ������.

#define TRIG_INPUT              (set.trig.input)                        // SettingsTrig.input
#define TRIG_INPUT_IS_AC        (TRIG_INPUT == TrigInput::AC)           // \c true, ���� �������� ���� �������������.

#define TRIG_LEVEL(source)      (set.trig.levelRel[source])             // SettingsTrig.levelRel

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
