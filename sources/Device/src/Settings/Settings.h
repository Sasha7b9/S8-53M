#pragma once
#include "Menu/Items/MenuItems.h"
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsDisplay.h"
#include "Settings/SettingsMemory.h"
#include "Settings/SettingsService.h"
#include "Settings/SettingsCursors.h"
#include "Settings/SettingsDebug.h"
#include "Settings/SettingsMemory.h"
#include "Settings/SettingsMeasures.h"
#include "Settings/SettingsMath.h"
#include "Utils/Measures.h"


#define SOUND_ENABLED       (set.service.soundEnabled)          // SettingsService.soundEnabled

#define RSHIFT_ADD(ch, range, closed) set.chan[ch].rShiftAdd[range][closed]


 
 // ��������� �������
struct SettingsChannel
{
    float         stretchADC;                 // ����������� �����������.
    int16         rShiftRel;
    int16         rShiftAdd[Range::Count][2]; // ���������� �������� ��� ��������� (0) � ��������� (1) ������.
    ModeCouple::E modeCouple;                 // ����� �� �����.
    Divider::E    divider;                    // ���������.
    Range::E      range;                      // ������� �� ����������.
    bool          enable;                     // ������� �� �����.
    bool          inverse;                    // ������������ �� �����.
    bool          filtr;                      // ������
    int8          balanceShiftADC;            // ���������� �������� ��� ������������ ���.
};




#define MAC_ADDR0       (set.eth.mac0)
#define MAC_ADDR1       (set.eth.mac1)
#define MAC_ADDR2       (set.eth.mac2)
#define MAC_ADDR3       (set.eth.mac3)
#define MAC_ADDR4       (set.eth.mac4)
#define MAC_ADDR5       (set.eth.mac5)
//
#define IP_ADDR0        (set.eth.ip0)
#define IP_ADDR1        (set.eth.ip1)
#define IP_ADDR2        (set.eth.ip2)
#define IP_ADDR3        (set.eth.ip3)
//
#define PORT_ETH        (set.eth.port)
//
#define NETMASK_ADDR0   (set.eth.mask0)
#define NETMASK_ADDR1   (set.eth.mask1)
#define NETMASK_ADDR2   (set.eth.mask2)
#define NETMASK_ADDR3   (set.eth.mask3)
//
#define GW_ADDR0        (set.eth.gw0)
#define GW_ADDR1        (set.eth.gw1)
#define GW_ADDR2        (set.eth.gw2)
#define GW_ADDR3        (set.eth.gw3)
//
#define ETH_ENABLE      (set.eth.enable)


struct SettingsEthernet
{
    uint8 mac0;
    uint8 mac1;
    uint8 mac2;
    uint8 mac3;
    uint8 mac4;
    uint8 mac5;

    uint8 ip0;
    uint8 ip1;
    uint8 ip2;
    uint8 ip3;

    uint16 port;

    uint8 mask0;
    uint8 mask1;
    uint8 mask2;
    uint8 mask3;

    uint8 gw0;
    uint8 gw1;
    uint8 gw2;
    uint8 gw3;

    bool enable;
};



#define LANG            (set.common.lang)       // SettingsCommon.lang
#define LANG_RU         (LANG == Language::Russian)
#define LANG_EN         (LANG == Language::English)
                                    
struct SettingsCommon
{
    int         countEnables;               // ���������� ���������. ������������� ��� ������ ���������.
    int         countErasedFlashData;       // ������� ��� �������� ������ ������ � ���������.
    int         countErasedFlashSettings;   // ������� ��� �������� ������ � ����������.
    int         workingTimeInSecs;          // ����� ������ � ��������.
    Language::E lang;                       // ���� ����.
};



struct OutputRegisters
{
    bool    all;            // ���������� �������� ���� ���������.
    bool    flag;           // �������� �� ���� ����������.
    bool    rShiftA;
    bool    rShiftB;
    bool    trigLev;
    bool    range[2];
    bool    chanParam[2];
    bool    trigParam;
    bool    tShift;
    bool    tBase;
};



#define NUM_STRINGS                         (set.debug.numStrings)

#define SIZE_FONT_CONSOLE                   (set.debug.sizeFont)

#define CONSOLE_IN_PAUSE                    (set.debug.consoleInPause)

#define BALANCE_ADC_TYPE                    (set.debug.balanceADCtype)
#define BALANCE_ADC_TYPE_IS_HAND            (BALANCE_ADC_TYPE == BalanceADCtype::Hand)

#define BALANCE_ADC(ch)                     (set.debug.balanceADC[ch])
#define BALANCE_ADC_A                       (BALANCE_ADC(ChA))
#define BALANCE_ADC_B                       (BALANCE_ADC(ChB))

#define DEBUG_STRETCH_ADC_TYPE              (set.debug.stretchADCtype)
#define DEBUG_STRETCH_ADC_TYPE_IS_DISABLED  (DEBUG_STRETCH_ADC_TYPE == StretchADCtype::Disable)
#define DEBUG_STRETCH_ADC_TYPE_IS_HAND      (DEBUG_STRETCH_ADC_TYPE == StretchADCtype::Hand)
#define DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS  (DEBUG_STRETCH_ADC_TYPE == StretchADCtype::Settings)

#define DEBUG_STRETCH_ADC(ch)               (set.debug.stretchADC[ch])
#define DEBUG_STRETCH_ADC_A                 (DEBUG_STRETCH_ADC(ChA))
#define DEBUG_STRETCH_ADC_B                 (DEBUG_STRETCH_ADC(ChB))

#define NUM_MEAS_FOR_GATES                  (set.debug.numMeasuresForGates)

#define ADD_SHIFT_T0                        (set.debug.shiftT0)

#define SHOW_STATS                          (set.debug.showStats)

#define NUM_AVE_FOR_RAND                    (::set.debug.numAveForRand)


#define IP_ADDRESS          (set.service.IPaddress)             // SettingsService.IPaddress

// ��������� ���� ������.
struct SettingsService
{ //-V802
    bool              screenWelcomeEnable; // ����� �� ������������ ����� ����������� ��� ��������� �������. \todo ������ ���� ��������.
    bool              soundEnabled;        // �������� �� �����.
    CalibratorMode::E calibrator;          // ����� ������ �����������.
    int8              IPaddress;           // IP-����� (��������).
    ColorScheme::E    colorScheme;         // �������� ������.
};


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


// ��������� ��� X.
struct SettingsTime
{ //-V802
    TBase::E            tBase;          // ������� �� �������.
    int16               tShiftRel;      // �������� �� �������.
    FunctionTime::E     function_time;
    TPos::E             tPos;           // �������� ������������� � ������.
    SampleType::E       sampleType;     // ��� ������� ��� ������ �������������.
    PeackDetMode::E     peakDet;        // ����� ������ �������� ���������
    bool                selfRecorder;   // ������� �� ����� ���������.
    ENUM_POINTS_FPGA::E oldNumPoints;   // \brief ����� ������������� � ����� �������� ���������, �������������
                                        // ���������� ����� � 1024, � ����  ���������� ��, ��� ����, ����� �����
                                        // ������������.
};


// ���������� ���������.
struct SettingsDebug
{
    int16             numStrings;                 // ����� ����� � �������.
    int8              sizeFont;                   // ������ ������ ������� - 0 - 5, 1 - 8,
    bool              consoleInPause;             // \brief ������� ����, ��� ������� ��������� � ������ �����. ����� ����� ��������, ��� ����� 
                                                  // ��������� ��� �� ���������� � �� ���������.
    BalanceADCtype::E balanceADCtype;             // ��� ������������.
    int16             balanceADC[2];              // �������� ��������������� �������� ��� ��� ������ ������������.
    StretchADCtype::E stretchADCtype;             // ��� �������� ������.
    int16             stretchADC[2];              // �������� �������� ������ ��� ������� ������.
    int16             numMeasuresForGates;        // ����� ��������� ��� �����.
    int16             shiftT0;                    // �������������� �������� �� ������� ��� ������ �������� ������ �������������.
    bool              showStats;                  // ���������� ���������� �� ������ (fps, ��������).
    int16             numAveForRand;              // �� �������� ���������� ��������� ������ � ������ �������������.
    bool              viewAlteraWrittingData;     // ���������� �� ������, ������ � �������.
    bool              viewAllAlteraWrittingData;  // \brief ���������� �� ��� ������, ������ � ������� (���� false, �� ��������� ������ ������� ����� 
                                                  // START, STOP �� ������������).
    int16             altShift;                   // ���������� �������� ��� ���������� ������� �������������� ������ �������������.
    int16             pretriggered;               // ������������ �������� ����������� ��� ������������ �������������.
    OutputRegisters   showRegisters;
};

struct SettingsMenu
{
    bool        pageDebugActive;                  // ������� �� ������ ������� � ����.
    unsigned    isShown : 1;                      // ���� ��������.
};


// �������� ������ ��� ��������� �������.
struct Settings
{
    uint                size;
    SettingsDisplay     display;            // ��������� �����������          (���� �������).
    SettingsChannel     chan[NumChannels];  // ��������� �������              (���� ����� 1 � ����� 2).
    SettingsTrig        trig;               // ��������� �������������        (���� �����).
    SettingsTime        time;               // ��������� ���������            (���� ���¨����).
    SettingsCursors     cursors;            // ��������� ��������� ���������  (���� �������).
    SettingsMemory      memory;             // ��������� ������� ������       (���� ������).
    SettingsMeasures    measures;           // ��������� ���������            (���� ���������).
    SettingsMath        math;               // ��������� ������ �������������� ���������.
    SettingsService     service;            // �������������� ���������       (���� ������).
    SettingsEthernet    eth;                // ��������� ��� ���������� �� ��������� ����.
    SettingsCommon      common;             // ��������� ���������.
    SettingsMenu        menu;               // ��������� ����.
    SettingsDebug       debug;              // ��������� ������ �������       (���� �������).
    uint                crc32;              // ����������� �����. ������������ ��� �������� ������������ ���������� ��������
    //int temp[5];
    static void Load(bool _default);  // \brief ��������� ���������. ���� _default == true, ����������� ��������� �� ���������, ����� �������� 
                                      // ��������� ��������� �� ���, � � ������ ������� - ���� ��������� �� ���������.
    static void Save();               // ��������� ��������� �� ����-������.
    static bool DebugModeEnable();    // ���������� true, ���� ������� ����� �������.
    static bool loaded;    // ��� ���������� ����� ��� ����, ����� ��������� ������ ������ ������ �������� ��-�� ������������� 
                                        // ��������� ������� (��� ����������� �� ����� ����������� ��������� ������ ������ �� ������ ���������� 
                                        // ������� �������� �������, ��� ��������� �������� ��������� ���������).
};

extern Settings set;
