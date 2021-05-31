// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Measures.h"


 // ��������� �������
struct SettingsChannel
{
    int16      rshift;
    ModeCouple mode_couple;                 // ����� �� �����.
    Divider    divider;                     // ���������.
    Range      range;                       // ������� �� ����������.
    bool       enable;                      // ������� �� �����.
    bool       inverse;                     // ������������ �� �����.
    bool       filtr;                       // ������
};


struct SettingsLAN
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

    bool enabled;
};

                                   
struct SettingsSystem
{
    int         countEnables;               // ���������� ���������. ������������� ��� ������ ���������.
    int         countErasedFlashData;       // ������� ��� �������� ������ ������ � ���������.
    int         countErasedFlashSettings;   // ������� ��� �������� ������ � ����������.
    int         workingTimeInSecs;          // ����� ������ � ��������.
    Language::E lang;                       // ���� ����.
};



struct StructMemoryLast
{
    bool isActiveModeSelect;
};


// ��������� ����->������
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    EnumPointsFPGA    enum_points_fpga;                         // ����� �����.
    ModeWork            mode_work;                                // ����� ������.
    FileNamingMode      file_naming_mode;                         // ����� ���������� ������.
    char                file_name_mask[MAX_SYMBOLS_IN_FILE_NAME]; // ����� ����� ��� ��������������� ���������� ������\n
        // ������� ����������.\n
        // %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47')
        // - �������\n
        // %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� 
        // ������������� � 007\n
        // �������\n
        // name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32\n
        // ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����,
        //���� �������� ����� ������ ����� 0001, �.�. ��� ����������� ������ ��������������� ������ ������� �� ����.
    char                file_name[MAX_SYMBOLS_IN_FILE_NAME];      // ��� ����� ��� ������ ������� �������
    int8                index_cur_symbol_name_mask;               // ������ �������� ������� � ������ ������� ����� ���
                                                                  // ������ �����.
    StructMemoryLast    str_memory_last;
    ModeShowIntMem      mode_show_int_mem;                        // ����� ������� ���������� � ������ ����������� ��
    bool                flash_auto_connect;                       // ���� true, ��� ���������� ���� �������������
                                                                  // ��������� NC (������ ���������)
    ModeBtnMemory       mode_button_memory;
    ModeSaveSignal      mode_save_signal;                         // � ����� ���� ��������� ������.
};


// ��������� �����������
struct SettingsDisplay
{
    int16               time_show_levels;           // �����, � ������� �������� ����� ���������� ������ ��������.
    int16               shift_in_memory_in_points;  // ���������� �������� ������ ���� ����� ������������ �������� �����
                                                    // ������. ����� ��� ����������� ����������� ������� � ����.
    int16               time_messages;              // ����� � ��������, �� ������� ��������� ������� �� ������.
    int16               brightness;                 // ������� �������.
    int16               brightness_grid;            // ������� ����� �� 0 �� 100.
    uint                colors[16];                 // �����.
    ModeDrawSignal      mode_draw_signal;           // ����� ��������� �������.
    TypeGrid            type_grid;                  // ��� �����.
    ENumAccumulation    enum_accum;                 // ������������ ���������� ������� �� ������.
    ENumAveraging       enum_ave;                   // ������������ ���������� �������.
    ModeAveraging       mode_ave;                   // ��� ���������� �� ����������.
    ENumMinMax          enum_min_max;               // ����� ��������� ��� ����������� ��������� � ����������.
    Smoothing           smoothing;                  // ����� ����� ��� ����������� �������.
    ENumSignalsInSec    enum_fps;                   // ������������ ���������� ������� � �������.
    Channel             last_affected_channel;      // ����� �������� ����� ���������� ������, ������� ��������� �������
                                                    // ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation    mode_acum;                  // ����� ����� ���������� ��������.
    AltMarkers          alt_markers;                // ����� ����������� �������������� ������� �������� ��������.
    MenuAutoHide        menu_auto_hide;           // ����� ������� ������� ����� ���������� ������� ������� ������� ����
    bool                show_full_memory_window;    // ���������� �� ���� ������ ������ ������. \todo �� ������������.
    bool                show_string_navigation;     // ���������� �� ������ �������� ��������� ����..
    LinkingRShift       linking_rshift;             // ��� �������� �������� �� ���������.

    // ���������� ����� ������ � ��������� ����� �� ������.
    static BitSet64 PointsOnDisplay();

    // ���������� ������ ������� � ���������� ������ �� ������
    static BitSet64 BytesOnDisplay();

    // ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();
};


// ��������� ��������� ���������.
struct SettingsCursors
{
    CursCntrl       cntrlU[Channel::Count];  // �������� ������� ����������.
    CursCntrl       cntrlT[Channel::Count];  // �������� ������� ����������.
    Channel         source;                  // �������� - � ������ ������ ��������� �������.
    float           posU[Channel::Count][2]; // ������� ������� �������� ���������� ����� �������.
    float           posT[Channel::Count][2]; // ������� ������� �������� ������� ����� �������.
    float           dU_100percents[2];       // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float           dT_100percents[2];       // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    CursMovement    movement;                // ��� ������������ �������� - �� ������ ��� �� ���������.
    CursActive      active;                  // ����� ������� ������ �������.
    CursLookMode    look_mode[2];            // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool            show_freq;               // ������������� � true ��������, ��� ����� ���������� �� ������ ��������
                                             // 1/dT ����� ���������.
    bool            show;                    // ���������� �� �������.
};


struct Cursors
{
    // �������� ������� ������� ����������.
    static float GetPosU(const Channel &ch, int num);

    // ���������� true,���� ����� �������� �������.
    static bool NecessaryDraw();

    // �������� ������ ������� ����������.
    static Voltage GetVoltage(const Channel &source, int num_cur);
    static Voltage GetVoltage(float shift_cur, Range::E range, int16 rshift);

    // �������� ������ ������� �������.
    static Time GetTime(const Channel &source, int numCur);
    static Time GetTime(float shift_cur, TBase::E tbase);

    // �������� ������ ��������� �������� ����������.
    static Text GetPercentsU(const Channel &source);

    // �������� ������ ��������� �������� �������.
    static Text GetPercentsT(const Channel &source);

    // ���������� ������� ��������� ���������.
    static void Draw();

private:

    // ���������� ������������ ������ ��������� ���������.
    // x         - �������� �������� �������
    // y_tearing - ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������
    static void DrawVertical(int x, int yTearing);

    // ���������� �������������� ������ ��������� ���������
    // y         - �������� �������� �������
    // x_tearing - ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������
    static void DrawHorizontal(int y, int x_tearing);
};


// ��������� ���� ���������
struct SettingsMeasures
{
    MeasuresNumber  number;            // ������� ��������� ��������.
    Channel         source;            // ��� ����� ������� �������� ���������.
    ModeViewSignals mode_view_signals; // ������� �� ������� ��� ������ ���������.
    Measure         measures[15];      // ��������� ��� ��������� ���������.
    bool            show;              // ���������� �� ���������.
    MeasuresField   field;             // ����� �������, �� ������� ������� �������� ��� ������� ���������.
    int16           posU[2];           // ������� ��������, ������� ������ �������, �� ������� ������� �������� ���
                                       // ������� ��������� ��� field == MeasuresField::Hand.
    int16           posT[2];           // ������� ��������, ������� ������ �������, �� ������� ������� �������� ���
                                       // ������� ��������� ��� field == MeasuresField::Hand.
    CursCntrl       cntrlU;            // �������� ������� ����������.
    CursCntrl       cntrlT;            // �������� ������� �������.
    CursActive      curs_active;       // ����� ������� ������� - �� ������� ��� ����������.
    Measure         marked;            // ���������, �� ������� ����� �������� �������.

    static int16 PosCursorU(int num);
    static int16 PosCursorT(int num);
};


struct SettingsMath
{
    struct FFT
    {
        ScaleFFT    scale;          // ������� ����� ��� ����������� �������.
        SourceFFT   source;         // �������� ������� ��� ������� � ����������� �������.
        WindowFFT   window;         // ����� ��� ����, �� ������� ���������� ������ ����� �������� �������.
        MaxDbFFT    max_DB;         // ����������� �������� �� ������������ ���������� ������� �������.
        uint8       cursor;         // ����������, ����� �������� ������� ��������� ����� ���������.
        uint8       pos_cur[2];     // ������� ������� �������. ���������� 0...256.
        bool        enabled;        // \c true, ���� ������� ������.
    } fft;
    Function        func;           // ���������� �������������� �������.
    int8            koeff1add;      // ����������� ��� ������ ��������� ��� ��������.
    int8            koeff2add;
    int8            koeff1mul;
    int8            koeff2mul;
    ModeDrawMath::E modeDraw;       // ���������� ��� ����� ������� � ������ �������������� �������.
    ModeRegSet::E   modeRegSet;     // ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
    Range           range;          // ������� �� ���������� ��������������� �������.
    Divider::E      multiplier;     // ��������� �������� ��������������� �������.
    int16           rShift;         // �������� �� ������������ ��� ��������������� �������.

    static bool IsMathEnabled();
};



// ��������� ���� ������.
struct SettingsService
{
    CalibratorMode::E calibrator;          // ����� ������ �����������.
    ColorScheme::E    colorScheme;         // �������� ������.
    bool              sound_enabled;       // �������� �� �����.
    int8              IPaddress;           // IP-����� (��������).
};


// ��������� �������������
struct SettingsTrig
{
    StartMode            start_mode;         // ����� �������.
    TrigSource::E        source;             // ��������.
    TrigPolarity         polarity;           // ��� �������������.
    TrigInput::E         input;              // ���� �������������.
    int16                levelRel[3];        // ������� ������������� ��� ��� ����������.
    ModeLongPressTrig::E modeLongPressTrig;  // ����� ������ ����������� ������� ������ �����.
    TrigModeFind::E      modeFind;           // ����� ������������� - ������� ��� �������������.
};


// ��������� ��� X.
struct SettingsTime
{
    TBase::E            base;           // ������� �� �������.
    FunctionTime::E     function_time;
    TPos                t_pos;          // �������� ������������� � ������.
    SampleType::E       sample_type;    // ��� ������� ��� ������ �������������.
    PeackDetMode        peak_det;       // ����� ������ �������� ���������
    EnumPointsFPGA::E old_num_points; // \brief ����� ������������� � ����� �������� ���������, �������������
                                        // ���������� ����� � 1024, � ����  ���������� ��, ��� ����, ����� �����
                                        // ������������.
    int                 shift;          // �������� �� �������.
    bool                self_recorder;  // ������� �� ����� ���������.
};


struct SettingsMenu
{
    bool        pageDebugActive;                  // ������� �� ������ ������� � ����.
    unsigned    isShown : 1;                      // ���� ��������.
};


// �������� ������ ��� ��������� �������.
struct SettingsMain
{
    uint             size;
    uint             crc32;
    uint             number;
    SettingsDisplay  display;              // ��������� �����������          (���� �������).
    SettingsChannel  chan[Channel::Count]; // ��������� �������              (���� ����� 1 � ����� 2).
    SettingsTrig     trig;                 // ��������� �������������        (���� �����).
    SettingsTime     time;                 // ��������� ���������            (���� ���¨����).
    SettingsCursors  cursors;              // ��������� ��������� ���������  (���� �������).
    SettingsMemory   memory;               // ��������� ������� ������       (���� ������).
    SettingsMeasures measures;             // ��������� ���������            (���� ���������).
    SettingsMath     math;                 // ��������� ������ �������������� ���������.
    SettingsService  service;              // �������������� ���������       (���� ������).
    SettingsLAN      LAN;                  // ��������� ��� ���������� �� ��������� ����.
    SettingsSystem   common;               // ��������� ���������.
    SettingsMenu     menu;                 // ��������� ����.
    uint             unused_crc32;

    void Load();

    void Save();

    void Reset();

    // ���������� � �������� ����������� �����
    uint CalcWriteCRC32();

    static bool DebugModeEnabled() { return true; }

    bool operator!=(const SettingsMain &rhs);
};


extern SettingsMain set;


#define MAC_ADDR0       (set.LAN.mac0)
#define MAC_ADDR1       (set.LAN.mac1)
#define MAC_ADDR2       (set.LAN.mac2)
#define MAC_ADDR3       (set.LAN.mac3)
#define MAC_ADDR4       (set.LAN.mac4)
#define MAC_ADDR5       (set.LAN.mac5)

#define IP_ADDR0        (set.LAN.ip0)
#define IP_ADDR1        (set.LAN.ip1)
#define IP_ADDR2        (set.LAN.ip2)
#define IP_ADDR3        (set.LAN.ip3)

#define NETMASK_ADDR0   (set.LAN.mask0)
#define NETMASK_ADDR1   (set.LAN.mask1)
#define NETMASK_ADDR2   (set.LAN.mask2)
#define NETMASK_ADDR3   (set.LAN.mask3)

#define GW_ADDR0        (set.LAN.gw0)
#define GW_ADDR1        (set.LAN.gw1)
#define GW_ADDR2        (set.LAN.gw2)
#define GW_ADDR3        (set.LAN.gw3)


#define LANG            (set.common.lang)       // SettingsMain.lang
#define LANG_RU         (LANG == Language::Russian)
#define LANG_EN         (LANG == Language::English)
