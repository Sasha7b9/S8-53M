#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Panel/Panel.h"
#include "Utils/Math.h"
#include "Utils/GlobalFunctions.h"
#include "Display/Display.h"
#include <cstring>



static const Settings defaultSettings =
{
    // Display
    {
        5,                          // timeShowLevels
        512 - 140,                  // shiftInMemory
        5,                          // timeMessages
        100,                        // brightness
        20,                         // brightnessGrid
        {
            MAKE_COLOR(0, 0, 0),
            MAKE_COLOR(31, 63, 31),
            MAKE_COLOR(15, 31, 15),
            MAKE_COLOR(1, 50, 31),
            MAKE_COLOR(0, 63, 0),
            MAKE_COLOR(26, 54, 26),
            MAKE_COLOR(31, 44, 0),
            MAKE_COLOR(15, 22, 0),
            MAKE_COLOR(31, 63, 0),
            MAKE_COLOR(26, 34, 0),
            MAKE_COLOR(13, 17, 0),
            MAKE_COLOR(31, 51, 0),
            MAKE_COLOR(0, 0, 0),
            MAKE_COLOR(0, 0, 0),
            MAKE_COLOR(1, 40, 25),
            MAKE_COLOR(0, 51, 0)
        },
        ModeDrawSignal::Lines,      // modeDrawSignal
        TypeGrid::_1,               // typeGrid
        ENumAccumulation::_1,       // numAccumulation
        ENumAveraging::_1,          // numAveraging
        ModeAveraging::Accurately,  // modeAveraging
        ENumMinMax::_1,             // numMinMax
        DisplaySmoothing::Disable,  // smoothing
        ENumSignalsInSec::_25,      // num signals in one second
        Channel::A,                 // lastAffectedChannel
        ModeAccumulation::NoReset,  // modeAccumulation
        AltMarkers::Show,           // altMarkers
        MenuAutoHide::None,         // menuAutoHide
        true,                       // showFullMemoryWindow
        false                       // showStringNavigation
    },
    // time
    {
        TBase::_200us,
        0,                          // set.time.tShiftRel
        FunctionTime::Time,
        TPos_Center,
        SampleType_Equal,
        PeackDetMode::Disable,
        false,
        FNP_1024
    },
    // cursors
    {
        { CursCntrl_Disable, CursCntrl_Disable },   // CursCntrl U
        { CursCntrl_Disable, CursCntrl_Disable },   // CursCntrl T
        Channel::A,                                 // source
        { 60.0F,  140.0F, 60.0F, 140.0F },          // posCur U
        { 80.0F,  200.0F, 80.0F, 200.0F },          // posCur T
        { 80.0F,  80.0F },                          // ���������� ����� ��������� ���������� ��� 100%
        { 120.0F, 120.0F },                         // ���������� ����� ��������� ������� ��� 100%
        CursMovement_Points,                        // CursMovement
        CursActive_None,                            // CursActive
        { CursLookMode_None, CursLookMode_None },   // ����� �������� ��������.
        false,                                      // showFreq
        false                                       // showCursors
    },
    // memory
    {
        FNP_1024,
        ModeWork_Direct,
        FileNamingMode_Mask,
        "Signal_\x07\x03",      // \x07 - ��������, ��� ����� ������ ���� ��������� �����, \x03 - ������� �� ��� ����������
        "Signal01",
        0,
        {
            false
        },
        ModeShowIntMem_Both,    // modeShowIntMem
        false,                  // flashAutoConnect
        ModeBtnMemory_Menu,
        ModeSaveSignal_BMP
    },
    // math
    {
        ScaleFFT::Log,
        SourceFFT_Channel0,
        WindowFFT::Rectangle,
        FFTmaxDB::_60,
        Function::Sum,
        0,
        {100, 256 - 100},
        1,
        1,
        1,
        1,
        false,
        ModeDrawMath::Disable,       // modeDrawMath
        ModeRegSet_Range,           // modeRegSet
        Range::_50mV,
        Multiplier_1,
        0
    },
    // service
    {
        true,                       // screenWelcomeEnable
        true,                       // soundEnable
        Calibrator_Freq,            // calibratorEnable
        0,                          // IP-����� (��������)  WARN
        ColorScheme::WhiteLetters    // colorScheme
    },
    {
        0x8b, 0x2e, 0xaf, 0x8f, 0x13, 0x00, // mac
        192, 168, 1, 200,                   // ip
        7,
        255, 255, 255, 0,                   // netmask
        192, 168, 1, 1                      // gateway
    },
    // common
    {
        0
    },
    // debug
    {
        0,                      // numStrings
        0,                      // ������ ������ - 5
        0,                      // consoleInPause
        BalanceADC_Settings,    // balanceADC
        {0, 5},                 // shiftADC
        StretchADC_Settings,    // stretch
        {128, 128},             // stretchADC
//      RShiftADC_Settings,
        1000,                   // numMeasuresForGates
        0,                      // shiftT0
        false,                  // showStats
        6,                      // numAveForRand
        false,                  // view altera writting data
        false,                  // view all altera writting data
        0,                      // alt shift
        {
            false,              // ���������� �� ���� ����������
            false,              // rShift0
            false,              // rShift1
            false               // trigLev
        }
    }
};

Settings set;



void LoadDefaultColors(void)
{
    for (int color = 0; color < Color::Count; color++)
    {
        set.display.colors[color] = defaultSettings.display.colors[color];
    }
}



void Settings_Load(void)
{
    set = defaultSettings;
}
