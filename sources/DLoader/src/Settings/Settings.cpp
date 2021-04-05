#include "defines.h"
#include "common/Display/Colors_.h"
#include "Settings/Settings.h"


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
            Color::Make(0, 0, 0),
            Color::Make(31, 63, 31),
            Color::Make(15, 31, 15),
            Color::Make(1, 50, 31),
            Color::Make(0, 63, 0),
            Color::Make(26, 54, 26),
            Color::Make(31, 44, 0),
            Color::Make(15, 22, 0),
            Color::Make(31, 63, 0),
            Color::Make(26, 34, 0),
            Color::Make(13, 17, 0),
            Color::Make(31, 51, 0),
            Color::Make(0, 0, 0),
            Color::Make(0, 0, 0),
            Color::Make(1, 40, 25),
            Color::Make(0, 51, 0)
        },
        ModeDrawSignal::Lines,      // modeDrawSignal
        TypeGrid::_1,               // typeGrid
        ENumAccumulation::_1,       // numAccumulation
        ENumAveraging::_1,          // numAveraging
        ModeAveraging::Accurately,  // modeAveraging
        ENumMinMax::_1,             // numMinMax
        Smoothing::Disable,         // smoothing
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
        false,
        0,                          // set.time.tShiftRel
        TBase::_200us,
        FunctionTime::Time,
        TPos::Center,
        SampleType::Equal,
        PeackDetMode::Disable,
        EnumPointsFPGA::_1k
    },
    // cursors
    {
        { CursCntrl::Disable, CursCntrl::Disable }, // CursCntrl U
        { CursCntrl::Disable, CursCntrl::Disable }, // CursCntrl T
        Channel::A,                                 // source
        { 60.0F,  140.0F, 60.0F, 140.0F },          // posCur U
        { 80.0F,  200.0F, 80.0F, 200.0F },          // posCur T
        { 80.0F,  80.0F },                          // ���������� ����� ��������� ���������� ��� 100%
        { 120.0F, 120.0F },                         // ���������� ����� ��������� ������� ��� 100%
        CursMovement::Points,                        // CursMovement
        CursActive::None,                           // CursActive
        { CursLookMode::None, CursLookMode::None },   // ����� �������� ��������.
        false,                                      // showFreq
        false                                       // showCursors
    },
    // memory
    {
        EnumPointsFPGA::_1k,
        ModeWork::Direct,
        FileNamingMode::Mask,
        "Signal_\x07\x03",      // \x07 - ��������, ��� ����� ������ ���� ��������� �����, \x03 - ������� �� ��� ����������
        "Signal01",
        0,
        {
            false
        },
        ModeShowIntMem::Both,    // modeShowIntMem
        false,                  // flashAutoConnect
        ModeBtnMemory::Menu,
        ModeSaveSignal::BMP
    },
    // math
    {
        ScaleFFT::Log,
        SourceFFT::A_,
        WindowFFT::Rectangle,
        MaxDbFFT::_60,
        Function::Sum,
        0,
        {100, 256 - 100},
        1,
        1,
        1,
        1,
        false,
        ModeDrawMath::Disable,       // modeDrawMath
        ModeRegSet::Range,           // modeRegSet
        Range::_50mV,
        Divider::_1,
        0
    },
    // service
    {
        true,                       // screenWelcomeEnable
        true,                       // soundEnable
        0,                          // IP-����� (��������)  WARN
        CalibratorMode::Freq,       // calibratorEnable
        ColorScheme::WhiteLetters   // colorScheme
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
        0,                          // numStrings
        0,                          // ������ ������ - 5
        0,                          // consoleInPause
        BalanceADCtype::Settings,   // balanceADC
        {0, 5},                     // shiftADC
        StretchADCtype::Settings,   // stretch
        {128, 128},                 // stretchADC
//      RShiftADC_Settings,
        1000,                       // numMeasuresForGates
        0,                          // shiftT0
        false,                      // showStats
        6,                          // numAveForRand
        false,                      // view altera writting data
        false,                      // view all altera writting data
        0,                          // alt shift
        {
            false,                  // ���������� �� ���� ����������
            false,                  // rShift0
            false,                  // rShift1
            false                   // trigLev
        }
    }
};

Settings set;



void LoadDefaultColors()
{
    for (int color = 0; color < Color::Count; color++)
    {
        set.display.colors[color] = defaultSettings.display.colors[color];
    }
}



void Settings_Load()
{
    set = defaultSettings;
}
