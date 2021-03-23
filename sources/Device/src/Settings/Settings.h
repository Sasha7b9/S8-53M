#pragma once
#include "Menu/Items/MenuItems.h"
#include "Utils/Measures.h"


 // ��������� �������
struct SettingsChannel
{
    float         stretch_ADC;                 // ����������� �����������.
    int16         rshift;
    int16         rshift_add[Range::Count][2]; // ���������� �������� ��� ��������� (0) � ��������� (1) ������.
    ModeCouple    mode_�ouple;                 // ����� �� �����.
    Divider       divider;                     // ���������.
    Range         range;                       // ������� �� ����������.
    bool          enable;                      // ������� �� �����.
    bool          inverse;                     // ������������ �� �����.
    bool          filtr;                       // ������
    int8          balance_shift_ADC;           // ���������� �������� ��� ������������ ���
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


struct StructMemoryLast
{
    bool isActiveModeSelect;
};


// ��������� ����->������
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENUM_POINTS_FPGA::E enum_points_fpga;                         // ����� �����.
    ModeWork::E         modeWork;                                 // ����� ������.
    FileNamingMode::E   fileNamingMode;                           // ����� ���������� ������.
    char                file_name_mask[MAX_SYMBOLS_IN_FILE_NAME]; // ����� ����� ��� ��������������� ���������� ������\n
        // ������� ����������.\n
        // %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������\n
        // %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007\n
        // �������\n
        // name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32\n
        // ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����,
        //���� �������� ����� ������ ����� 0001, �.�. ��� ����������� ������ ��������������� ������ ������� �� ����.
    char                file_name[MAX_SYMBOLS_IN_FILE_NAME];      // ��� ����� ��� ������ ������� �������
    int8                index_cur_symbol_name_mask;               // ������ �������� ������� � ������ ������� ����� ���
                                                                  // ������ �����.
    StructMemoryLast    strMemoryLast;
    ModeShowIntMem::E   modeShowIntMem;                           // ����� ������� ���������� � ������ ����������� ��
    bool                flash_auto_connect;                       // ���� true, ��� ���������� ���� �������������
                                                                  // ��������� NC (������ ���������)
    ModeBtnMemory::E    modeBtnMemory;
    ModeSaveSignal::E   modeSaveSignal;                           // � ����� ���� ��������� ������.

    static int GetNumPoints(bool forCalculate);
    static ENUM_POINTS_FPGA::E IntNumPoints2FPGA_NUM_POINTS(int numPoints);
};


// ��������� �����������
struct SettingsDisplay
{
    int16               time_show_levels;        // �����, � ������� �������� ����� ���������� ������ ��������.
    int16               shift_in_memory;         // ���������� �������� ������ ���� ����� ������������ �������� �����
                                                 // ������. ����� ��� ����������� ����������� ������� � ����.
    int16               time_messages;           // ����� � ��������, �� ������� ��������� ������� �� ������.
    int16               brightness;              // ������� �������.
    int16               brightness_grid;         // ������� ����� �� 0 �� 100.
    uint                colors[16];              // �����.
    ModeDrawSignal      mode_draw_signal;        // ����� ��������� �������.
    TypeGrid            type_grid;               // ��� �����.
    ENumAccumulation    enum_accum;              // ������������ ���������� ������� �� ������.
    ENumAveraging       enum_ave;                // ������������ ���������� �������.
    ModeAveraging       mode_ave;                // ��� ���������� �� ����������.
    ENumMinMax          enum_min_max;            // ����� ��������� ��� ����������� ��������� � ����������.
    Smoothing           smoothing;               // ����� ����� ��� ����������� �������.
    ENumSignalsInSec    enum_fps;                // ������������ ���������� ������� � �������.
    Channel             last_affected_channel;   // ����� �������� ����� ���������� ������, ������� ��������� �������.
                                                 // ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation    mode_acum;               // ����� ����� ���������� ��������.
    AltMarkers          alt_markers;             // ����� ����������� �������������� ������� �������� ��������.
    MenuAutoHide        menu_auto_hide;          // ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    bool                show_full_memory_window; // ���������� �� ���� ������ ������ ������. \todo �� ������������.
    bool                show_string_navigation;  // ���������� �� ������ �������� ��������� ����..
    LinkingRShift       linking_rshift;          // ��� �������� �������� �� ���������.

    // ���������� ����� ������ � ��������� ����� �� ������.
    static Set2Int PointsOnDisplay();

    // ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();
};


// ��������� ��������� ���������.
struct SettingsCursors
{
    CursCntrl       cntrlU[NumChannels];    // �������� ������� ����������.
    CursCntrl       cntrlT[NumChannels];    // �������� ������� ����������.
    Channel         source;                 // �������� - � ������ ������ ��������� �������.
    float           posU[NumChannels][2];   // ������� ������� �������� ���������� ����� �������.
    float           posT[NumChannels][2];   // ������� ������� �������� ������� ����� �������.
    float           dU_100percents[2];      // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float           dT_100percents[2];      // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    CursMovement    movement;               // ��� ������������ �������� - �� ������ ��� �� ���������.
    CursActive      active;                 // ����� ������� ������ �������.
    CursLookMode    look_mode[2];           // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool            show_freq;              // ������������� � true ��������, ��� ����� ���������� �� ������ ��������
                                            // 1/dT ����� ���������.
    bool            show;                   // ���������� �� �������.
};


struct Cursors
{
    // �������� ������� ������� ����������.
    static float GetPosU(const Channel &ch, int num);

    // ���������� true,���� ����� �������� �������.
    static bool NecessaryDraw();

    // �������� ������ ������� ����������.
    static String GetVoltage(const Channel &source, int numCur);

    // �������� ������ ������� �������.
    static String GetTime(const Channel &source, int numCur);

    // �������� ������ ��������� �������� ����������.
    static String GetPercentsU(const Channel &source);

    // �������� ������ ��������� �������� �������.
    static String GetPercentsT(const Channel &source);

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
    bool              sound_enabled;       // �������� �� �����.
    CalibratorMode::E calibrator;          // ����� ������ �����������.
    int8              IPaddress;           // IP-����� (��������).
    ColorScheme::E    colorScheme;         // �������� ������.
};


// ��������� �������������
struct SettingsTrig
{
    StartMode::E         start_mode;         // ����� �������.
    TrigSource::E        source;             // ��������.
    TrigPolarity::E      polarity;           // ��� �������������.
    TrigInput::E         input;              // ���� �������������.
    int16                levelRel[3];        // ������� ������������� ��� ��� ����������.
    ModeLongPressTrig::E modeLongPressTrig;  // ����� ������ ����������� ������� ������ �����.
    TrigModeFind::E      modeFind;           // ����� ������������� - ������� ��� �������������.
};


// ��������� ��� X.
struct SettingsTime
{
    TBase::E            base;           // ������� �� �������.
    int16               shift;          // �������� �� �������.
    FunctionTime::E     function_time;
    TPos::E             t_pos;          // �������� ������������� � ������.
    SampleType::E       sample_type;    // ��� ������� ��� ������ �������������.
    PeackDetMode::E     peak_det;       // ����� ������ �������� ���������
    bool                self_recorder;  // ������� �� ����� ���������.
    ENUM_POINTS_FPGA::E old_num_points; // \brief ����� ������������� � ����� �������� ���������, �������������
                                        // ���������� ����� � 1024, � ����  ���������� ��, ��� ����, ����� �����
                                        // ������������.
};


// ���������� ���������.
struct SettingsDebug
{
    int16             num_srings;                 // ����� ����� � �������.
    int8              size_font;                  // ������ ������ ������� - 0 - 5, 1 - 8,
    bool              console_in_pause;           // \brief ������� ����, ��� ������� ��������� � ������ �����. �����
                                                  // ����� ��������, ��� ����� 
                                                  // ��������� ��� �� ���������� � �� ���������.
    BalanceADCtype::E balance_ADC_type;           // ��� ������������.
    int16             balance_ADC[2];             // �������� ��������������� �������� ��� ��� ������ ������������.
    StretchADCtype::E stretch_ADC_type;           // ��� �������� ������.
    int16             stretch_ADC[2];             // �������� �������� ������ ��� ������� ������.
    int16             num_measures_for_gates;     // ����� ��������� ��� �����.
    int16             shift_T0;                   // �������������� �������� �� ������� ��� ������ �������� ������
                                                  // �������������.
    bool              show_stats;                 // ���������� ���������� �� ������ (fps, ��������).
    int16             num_ave_for_rand;           // �� �������� ���������� ��������� ������ � ������ �������������.
    bool              viewAlteraWrittingData;     // ���������� �� ������, ������ � �������.
    bool              viewAllAlteraWrittingData;  // \brief ���������� �� ��� ������, ������ � ������� (���� false, ��
                                                  // ��������� ������ ������� ����� 
                                                  // START, STOP �� ������������).
    int16             altShift;                   // ���������� �������� ��� ���������� ������� �������������� ������
                                                  // �������������.
    int16             pretriggered;               // ������������ �������� ����������� ��� ������������ �������������.
    OutputRegisters   showRegisters;

    static int  GetSizeFontForConsole();        // ���������� ������ ������, ������� ����� �������� ��������� � �������.
};

struct SettingsMenu
{
    bool        pageDebugActive;                  // ������� �� ������ ������� � ����.
    unsigned    isShown : 1;                      // ���� ��������.
};


// �������� ������ ��� ��������� �������.
struct Settings
{
    uint             size;
    SettingsDisplay  display;            // ��������� �����������          (���� �������).
    SettingsChannel  chan[NumChannels];  // ��������� �������              (���� ����� 1 � ����� 2).
    SettingsTrig     trig;               // ��������� �������������        (���� �����).
    SettingsTime     time;               // ��������� ���������            (���� ���¨����).
    SettingsCursors  cursors;            // ��������� ��������� ���������  (���� �������).
    SettingsMemory   memory;             // ��������� ������� ������       (���� ������).
    SettingsMeasures measures;           // ��������� ���������            (���� ���������).
    SettingsMath     math;               // ��������� ������ �������������� ���������.
    SettingsService  service;            // �������������� ���������       (���� ������).
    SettingsLAN      LAN;                // ��������� ��� ���������� �� ��������� ����.
    SettingsCommon   common;             // ��������� ���������.
    SettingsMenu     menu;               // ��������� ����.
    SettingsDebug    debug;              // ��������� ������ �������       (���� �������).
    uint             crc32;              // ����������� �����. ������������ ��� �������� ������������ ����������
                                         // ��������

    static void Load(bool _default);  // \brief ��������� ���������. ���� _default == true, ����������� ��������� ��
                                      // ���������, ����� �������� 
                                      // ��������� ��������� �� ���, � � ������ ������� - ���� ��������� �� ���������.
    static void Save();               // ��������� ��������� �� ����-������.
    static bool DebugModeEnable();    // ���������� true, ���� ������� ����� �������.
    static bool loaded;               // ��� ���������� ����� ��� ����, ����� ��������� ������ ������ ������ ��������
                                      // ��-�� ������������� ��������� ������� (��� ����������� �� ����� �����������
                                      // ��������� ������ ������ �� ������ ���������� ������� �������� �������, ���
                                      // ��������� �������� ��������� ���������).
};


extern Settings set;


#define RSHIFT_ADD(ch, range, mode) set.chan[ch].rshift_add[range][mode]

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


#define LANG            (set.common.lang)       // SettingsCommon.lang
#define LANG_RU         (LANG == Language::Russian)
#define LANG_EN         (LANG == Language::English)


#define IS_SHOW_REGISTERS_ALL   (set.debug.showRegisters.all)
#define IS_SHOW_REG_RSHIFT_A    (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.rShiftA)
#define IS_SHOW_REG_RSHIFT_B    (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.rShiftB)
#define IS_SHOW_REG_TRIGLEV     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.trigLev)
#define IS_SHOW_REG_RANGE_A     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.range[Channel::A])
#define IS_SHOW_REG_RANGE_B     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.range[Channel::B])
#define IS_SHOW_REG_TRIGPARAM   (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.trigParam)
#define IS_SHOW_REG_PARAM_A     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.chanParam[Channel::A])
#define IS_SHOW_REG_PARAM_B     (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.chanParam[Channel::B])
#define IS_SHOW_REG_TSHIFT      (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.tShift)
#define IS_SHOW_REG_TBASE       (IS_SHOW_REGISTERS_ALL || set.debug.showRegisters.tBase)
