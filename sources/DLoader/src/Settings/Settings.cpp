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
        ENUM_POINTS_FPGA::_1024
    },
    // cursors
    {
        { CursCntrl::Disable, CursCntrl::Disable }, // CursCntrl U
        { CursCntrl::Disable, CursCntrl::Disable }, // CursCntrl T
        Channel::A,                                 // source
        { 60.0F,  140.0F, 60.0F, 140.0F },          // posCur U
        { 80.0F,  200.0F, 80.0F, 200.0F },          // posCur T
        { 80.0F,  80.0F },                          // рассто€ние между курсорами напр€жени€ дл€ 100%
        { 120.0F, 120.0F },                         // рассто€ние между курсорами времени дл€ 100%
        CursMovement::Points,                        // CursMovement
        CursActive::None,                           // CursActive
        { CursLookMode::None, CursLookMode::None },   // –ежим слежени€ курсоров.
        false,                                      // showFreq
        false                                       // showCursors
    },
    // memory
    {
        ENUM_POINTS_FPGA::_1024,
        ModeWork::Direct,
        FileNamingMode::Mask,
        "Signal_\x07\x03",      // \x07 - означает, что здесь должен быть очередной номер, \x03 - минимум на три знакоместа
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
        SourceFFT::A,
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
        ModeRegSet::Range,           // modeRegSet
        Range::_50mV,
        Divider::_1,
        0
    },
    // service
    {
        true,                       // screenWelcomeEnable
        true,                       // soundEnable
        0,                          // IP-адрес (временно)  WARN
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
        0,                          // размер шрифта - 5
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
            false,                  // показывать ли флаг готовности
            false,                  // rShift0
            false,                  // rShift1
            false                   // trigLev
        }
    }
};

Settings set;



void LoadDefaultColors(void)
{
    for (int color = 0; color < Color::NUM; color++)
    {
        set.display.colors[color] = defaultSettings.display.colors[color];
    }
}



void Settings_Load(void)
{
    set = defaultSettings;
}
