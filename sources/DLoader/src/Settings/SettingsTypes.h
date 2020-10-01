#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SDisplay
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ColorScheme
{
    enum E
    {
        WhiteLetters,       // � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����
        BlackLetters        // � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������
    };
};

// ����� ��������� �������.
struct ModeDrawSignal
{
    enum E
    {
        Lines = 0,   // �������
        Points = 1   // �������
    };
};

// ��� ����� �� ������.
struct TypeGrid
{
    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Count
    };
};

// ���������� ����������.
struct ENumAccumulation
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _Infinity
    };
};

// ���������� ���������� �� ����������.
struct ENumAveraging
{
    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        _512
    };
};

// ��� ���������� �� ����������
enum ModeAveraging
{
    Averaging_Accurately,   // ��������� �����.
    Averaging_Around        // ��������� ��������������.
};

// ���������� ��������� ��� ������� ������������ � ������������� ��������.
enum NumMinMax
{
    NumMinMax_1,
    NumMinMax_2,
    NumMinMax_4,
    NumMinMax_8,
    NumMinMax_16,
    NumMinMax_32,
    NumMinMax_64,
    NumMinMax_128
};

// ���������� ����� ��� ������� �����������.
enum DisplaySmoothing
{
    DisplaySmooth_Disable,
    DisplaySmooth_2points,
    DisplaySmooth_3points,
    DisplaySmooth_4points,
    DisplaySmooth_5points,
    DisplaySmooth_6points,
    DisplaySmooth_7points,
    DisplaySmooth_8points,
    DisplaySmooth_9points,
    DisplaySmooth_10points
};

// ����������� FPS.
enum NumSignalsInSec
{
    NumSignalsInSec_25,
    NumSignalsInSec_10,
    NumSignalsInSec_5,
    NumSignalsInSec_2,
    NumSignalsInSec_1
};

// ����� ����������� ����������� �������� ��� ����������� ������� ������.
enum MissedSignals
{
    Missed_Hide,    // �� �������� �� �����
    Missed_Show,    // �������� �� �����
    Missed_Average  // ���������� � �������� �� �����
};

enum ModeAccumulation
{
    ModeAccumulation_NoReset,   // � ���� ������ ������������ ������ N ���������
    ModeAccumulation_Reset      // � ���� ������ ���������� N ��������� � ����� ������������
};

// ����� ����������� �������������� ������� �������� �������� �� ���������� 
enum AltMarkers
{
    AM_Hide,        // ������ �� ��������
    AM_Show,        // ������ ��������
    AM_AutoHide     // �������� � ������� ����� timeShowLevels
};

// ����� ����� ����� ����� ���������� ������� ������ �������� ����
enum MenuAutoHide
{
    MenuAutoHide_None    = 0,    // �������
    MenuAutoHide_5       = 5,    // ����� 5 ������
    MenuAutoHide_10      = 10,   // ����� 10 ������
    MenuAutoHide_15      = 15,   // ����� 15 ������
    MenuAutoHide_30      = 30,   // ����� 30 ������
    MenuAutoHide_60      = 60    // ����� 60 ������
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChannelX
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Channel
{
    ChanA,
    ChanB,
    ChanA_B,
    ChanMath
};

#define NumChannels 2

// ����� ������ �� �����
enum ModeCouple
{
    ModeCouple_DC,      // �������� ����
    ModeCouple_AC,      // �������� ����
    ModeCouple_GND      // ���� �������
};

// ��������
enum Multiplier
{
    Multiplier_1,
    Multiplier_10
};

// ������� �� ����������
enum Range
{
    Range_2mV,
    Range_5mV,
    Range_10mV,
    Range_20mV,
    Range_50mV,
    Range_100mV,
    Range_200mV,
    Range_500mV,
    Range_1V,
    Range_2V,
    Range_5V,
    Range_10V,
    Range_20V,
    RangeSize
};

// ����� �������
enum StartMode
{
    StartMode_Auto,     // ��������������
    StartMode_Wait,     // ������ 
    StartMode_Single    // �����������
};

// �������� �������������
enum TrigSource
{
    TrigSource_Channel0,    // ����� 1
    TrigSource_Channel1,    // ����� 2
    TrigSource_Ext          // �������
};

// ��� ������������
enum TrigPolarity
{
    TrigPolarity_Front,     // �� ������
    TrigPolarity_Back       // �� �����
};

// ���� �������������
enum TrigInput
{
    TrigInput_Full,         // ������ ������
    TrigInput_AC,           // ����������
    TrigInput_LPF,          // ���
    TrigInput_HPF           // ���
};

enum TrigModeFind
{
    TrigModeFind_Hand,          // ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������
    TrigModeFind_Auto           // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
};

// ����� ����������� ������� ������ �����
enum ModeLongPressTrig
{
    ModeLongPRessTrig_Level0,
    ModeLongPressTrig_Auto
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������� ��/���
struct FunctionTime { enum E
{
    Time,
    ShiftInMemory
}; };

// ������� �� �������
enum TBase
{
    TBase_2ns,
    TBase_5ns,
    TBase_10ns,
    TBase_20ns,
    TBase_50ns,
    TBase_100ns,
    TBase_200ns,
    TBase_500ns,
    TBase_1us, 
    TBase_2us,
    TBase_5us,
    TBase_10us,
    TBase_20us,
    TBase_50us,
    TBase_100us,
    TBase_200us,
    TBase_500us,
    TBase_1ms,
    TBase_2ms,
    TBase_5ms,
    TBase_10ms,
    TBase_20ms,
    TBase_50ms,
    TBase_100ms,
    TBase_200ms,
    TBase_500ms,
    TBase_1s,
    TBase_2s,
    TBase_5s,
    TBase_10s,
    TBaseSize
};

// ��������� ����� ������������� �� �������
enum TPos
{
    TPos_Left,      // �������� � ������ ����
    TPos_Center,    // �������� � ������
    TPos_Right      // �������� � ������� ����
};

// ��� ������� ��� ������ �������������.
enum SampleType
{
    SampleType_Real,   // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������
    SampleType_Equal   // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������
};

enum PeackDetMode
{
    PeackDet_Disable,
    PeackDet_Enable,
    PeackDet_Average
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cursors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����� �������� ���������
enum CursCntrl
{
    CursCntrl_1,        // ������
    CursCntrl_2,        // ������
    CursCntrl_1_2,      // ������
    CursCntrl_Disable   // �������
};

// ������������ ����������� ��������
enum CursMovement
{
    CursMovement_Points,    // �� ������
    CursMovement_Percents   // �� ���������
};

// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
enum CursActive
{
    CursActive_U,
    CursActive_T,
    CursActive_None
};

// ����� �������� ��������
enum CursLookMode
{
    CursLookMode_None,          // ������� �� ������
    CursLookMode_Voltage,       // ������� ������ �� ����������� �������������
    CursLookMode_Time,          // ������� ������ �� �������� �������������
    CursLookMode_Both           // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����� ����� �������, � ������� ��� ������.
enum FPGA_NUM_POINTS
{
    FNP_281,
    FNP_512,
    FNP_1024
};

// ����� ������.
enum ModeWork
{
    ModeWork_Direct,        // �������� �����.
    ModeWork_Latest,        // � ���� ������ ����� ����������� ��������� ���������� ���������
    ModeWork_MemInt,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������
};

// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������
enum ModeShowIntMem
{
    ModeShowIntMem_Direct,  // ���������� ������ ��������� �������.
    ModeShowIntMem_Saved,   // ���������� ���������� ������
    ModeShowIntMem_Both     // ���������� ������ ��������� ������� � ���������� ������
};

// ��� ������ ��� ������� ������ ������
enum ModeBtnMemory
{
    ModeBtnMemory_Menu,     // ����� ������������ ��������������� �������� ����
    ModeBtnMemory_Save      // ���������� ����������� ������ �� ������
};

// ����� ������������ ������
enum FileNamingMode
{
    FileNamingMode_Mask,        // ��������� �� �����
    FileNamingMode_Manually     // ��������� �������
};

// ��� ��������� ������ �� ������
enum ModeSaveSignal
{
    ModeSaveSignal_BMP,
    ModeSaveSignal_TXT
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Measures
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������� �� ������� ��� ������ ���������.
enum ModeViewSignals
{
    ModeViewSignals_AsIs,       // ���������� ������� ��� ����
    ModeViewSignals_Compress    // ������� ����� � ���������
};

enum MeasuresNumber
{
    MN_1,                       // 1 ��������� ����� �����
    MN_2,                       // 2 ��������� ����� �����
    MN_1_5,                     // 1 ������ � 5 �����������
    MN_2_5,                     // 2 ������ �� 5 ���������
    MN_3_5,                     // 3 ������ �� 5 ���������
    MN_6_1,                     // 6 ����� �� 1 ���������
    MN_6_2                      // 6 ����� �� 2 ���������
};

// ����, �� ������� ��������� ��������
enum MeasuresField
{
    MeasuresField_Screen,       // ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    MeasuresField_AllMemory,    // ��������� ����� ������������� �� ����� �������.
    MeasuresField_Hand          // ��������� ����� ������������� �� ����, ����������� �������������.
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum Function
{
    Function_Sum,
    Function_Mul
};

enum ScaleMath
{
    ScaleMath_Channel0,     // ������� ������ �� ������ 1
    ScaleMath_Channel1,     // ������� ������ �� ������ 2
    ScaleMath_Hand          // ������� ������� �������.
};

struct ScaleFFT { enum E
{
    Log,           // ��� �������� �������� ��������������� ����� ������ �������
    Linear         // ��� �������� �������� �������� ����� ������ �������
}; };

enum SourceFFT
{
    SourceFFT_Channel0,
    SourceFFT_Channel1,
    SourceFFT_Both
};

enum WindowFFT
{
    WindowFFT_Rectangle,
    WindowFFT_Hamming,
    WindowFFT_Blackman,
    WindowFFT_Hann
};

enum FFTmaxDB
{
    FFTmaxDB_40,
    FFTmaxDB_60,
    FFTmaxDB_80
};

enum ModeDrawMath
{
    ModeDrawMath_Disable,
    ModeDrawMath_Separate,
    ModeDrawMath_Together
};

enum ModeRegSet
{
    ModeRegSet_Range,
    ModeRegSet_RShift
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Service
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���� ����
enum Language
{
    Russian,    // �������
    English     // ����������
};

enum CalibratorMode
{
    Calibrator_Freq,
    Calibrator_DC,
    Calibrator_GND
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
enum BalanceADCtype
{
    BalanceADC_Disable,     // ������������ ���������.
    BalanceADC_Settings,    // ������������ �������� ������������, ������� �������� �������������.
    BalanceADC_Hand         // ������������ �������� ������������, �������� �������.
};

// ��� �������� ���
enum StretchADCtype
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
};

// ��� ��������������� ��������
enum RShiftADCtype
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
};
