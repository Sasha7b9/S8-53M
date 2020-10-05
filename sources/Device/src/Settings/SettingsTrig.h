#pragma once
//#include "SettingsTypes.h"


/** @addtogroup Settings
 *  @{
 *  @addtogroup SettingsTrig
 *  @{
 */



#define START_MODE              (set.trig.startMode)                    // SettingsTrig.startMode
#define START_MODE_IS_SINGLE    (START_MODE == StartMode::Single)       // \c true, ���� ����������� ����� �������.
#define START_MODE_IS_AUTO      (START_MODE == StartMode::Auto)         // \c true, ���� �������������� ����� �������.

#define TRIG_SOURCE             (set.trig.source)                       // SettingsTrig.source
#define TRIG_SOURCE_IS_EXT      (TRIG_SOURCE == TrigSource::Ext)        // \c true, ���� ������� �������� �������������.

#define TRIG_POLARITY           (set.trig.polarity)                     // SettingsTrig.polarity
#define TRIG_POLARITY_IS_FRONT  (TRIG_POLARITY == TrigPolarity::Front)  // \c true, ���� ������������� �� ������.

#define TRIG_INPUT              (set.trig.input)                        // SettingsTrig.input
#define TRIG_INPUT_IS_AC        (TRIG_INPUT == TrigInput_AC)            // \c true, ���� �������� ���� �������������.

#define TRIG_LEVEL(source)      (set.trig.levelRel[source])             // SettingsTrig.levelRel
#define TRIG_LEVEL_SOURCE       (TRIG_LEVEL(TRIG_SOURCE))               // set.trig.levelRel[set.trig.source]
#define TRIG_LEVEL_A            (TRIG_LEVEL(Channel::A))                // set.trig.levelRel[Channel::A]
#define TRIG_LEVEL_B            (TRIG_LEVEL(Channel::B))                // set.trig.levelRel[Channel::B]

#define MODE_LONG_PRESS_TRIG    (set.trig.modeLongPressTrig)            // SettingsTrig.modeLongPressTrig
// ���� \c true, �� ���������� ������� ������ \b ����� ���������� ������� ������������� � 0.
#define MODE_LONG_PRESS_TRIG_IS_LEVEL0  (MODE_LONG_PRESS_TRIG == ModeLongPRessTrig_Level0)

#define TRIG_MODE_FIND          (set.trig.modeFind)                     // SettingsTrig.modeFind
#define TRIG_MODE_FIND_IS_AUTO  (TRIG_MODE_FIND == TrigModeFind_Auto)   // \c true, ���� �������������� ��������� �������������.
#define TRIG_MODE_FIND_IS_HAND  (TRIG_MODE_FIND == TrigModeFind_Hand)   // \c true, ���� ������������� ��������������� �������.




// ����� �������.
struct StartMode { enum E
{
    Auto,             // ��������������.
    Wait,             // ������.
    Single            // �����������.
};};

// �������� �������������.
struct TrigSource { enum E
{
    A,      // ����� 1.
    B,      // ����� 2.
    Ext     // �������.
};};

// ��� ������������
struct TrigPolarity { enum E
{
    Front,  // ������������� �� ������.
    Back    // ������������� �� �����.
};};

// ���� �������������.
enum TrigInput
{
    TrigInput_Full,             // ������ ������.
    TrigInput_AC,               // ����������.
    TrigInput_LPF,              // ���.
    TrigInput_HPF               // ���.
};

// ����� ����������� ������� ������ �����.
enum ModeLongPressTrig
{
    ModeLongPRessTrig_Level0,   // ����� ������ ������������� � 0.
    ModeLongPressTrig_Auto      // �������������� ����� ������������� - ������� ��������������� ���������� ����� ���������� � ���������.
};


// ����� ��������� �������������.
enum TrigModeFind
{
    TrigModeFind_Hand,          // ������� ������������� ��������������� �������.
    TrigModeFind_Auto           // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
};




// ��������� �������������
struct SettingsTrig
{
    StartMode::E        startMode;          // ����� �������.
    TrigSource::E       source;             // ��������.
    TrigPolarity::E     polarity;           // ��� �������������.
    TrigInput           input;              // ���� �������������.
    int16               levelRel[3];        // ������� ������������� ��� ��� ����������.
    ModeLongPressTrig   modeLongPressTrig;  // ����� ������ ����������� ������� ������ �����.
    TrigModeFind        modeFind;           // ����� ������������� - ������� ��� �������������.
};


/** @}  @}
 */
