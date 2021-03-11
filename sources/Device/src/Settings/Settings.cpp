#include "defines.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "Hardware/EPROM.H"
#include "Menu/Menu.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstring>


bool Settings::loaded = false;

//MAKE_COLOR(31, 63, 31),  // Color::WHITE = 0x01,


static const Settings defaultSettings =
{
    0,                              // size
    // Display
    {
        5,                          // timeShowLevels
        512 - 140,                  // shiftInMemory
        5,                          // timeMessages
        100,                        // brightness
        20,                         // brightnessGrid
        {
            Color::Make(0x00, 0x00, 0x00),   // Color::BLACK = 0x00,
            Color::Make(0xFF, 0xFF, 0xFF),   // Color::WHITE = 0x01,
            Color::Make(0x80, 0x80, 0x80),   // Color::GRID = 0x02,
            Color::Make(0x01, 0xCA, 0xFF),   // Color::DATA_A = 0x03,
            Color::Make(0x00, 0xFF, 0x00),   // COLOR_DATA_B = 0x04,
            Color::Make(0xD5, 0xDA, 0xD5),   // Color::MENU_FIELD = 0x05,
            Color::Make(0xFF, 0xB2, 0x00),   // COLOR_MENU_TITLE = 0x06,
            Color::Make(0x7B, 0x59, 0x00),   // Color::MENU_TITLE_DARK = 0x07,
            Color::Make(0xFF, 0xFF, 0x00),   // Color::MENU_TITLE_BRIGHT = 0x08,
            Color::Make(0x69, 0x89, 0x00),   // COLOR_MENU_ITEM = 0x09,
            Color::Make(0x6B, 0x45, 0x00),   // Color::MENU_ITEM_DARK = 0x0a,
            Color::Make(0xFF, 0xCE, 0x00),   // Color::MENU_ITEM_BRIGHT = 0x0b,
            Color::Make(0x00, 0x00, 0x00),   // Color::MENU_SHADOW = 0x0c,
            Color::Make(0x00, 0x00, 0x00),   // COLOR_EMPTY = 0x0d,
            Color::Make(0x08, 0xA2, 0xCE),   // COLOR_EMPTY_A = 0x0e,
            Color::Make(0x00, 0xCE, 0x00)    // COLOR_EMPTY_B = 0x0f,
        },
        ModeDrawSignal::Lines,      // modeDrawSignal
        TypeGrid::_1,               // typeGrid
        ENumAccumulation::_1,       // numAccumulation
        ENumAveraging::_1,          // numAveraging
        ModeAveraging::Accurately,  // modeAveraging
        ENumMinMax::_1,             // enumMinMax
        Smoothing::Disable,         // smoothing
        ENumSignalsInSec::_25,      // num signals in one second
        Channel::A,                          // lastAffectedChannel
        ModeAccumulation::NoReset,  // modeAccumulation
        AltMarkers::Show,           // altMarkers
        MenuAutoHide::None,         // menuAutoHide
        true,                       // showFullMemoryWindow
        false,                      // showStringNavigation
        LinkingRShift::Voltage      // linkingRShift
    },
    // channels
    {
        {
            1.0F,                   // коэффициент калибровки
            RShiftZero,
            {{0}, {0}},             // rShiftAdd
            ModeCouple::AC,          // ModCouple
            Divider::_1,              // Divider
            Range::_500mV,            // range
            true,                   // enable
            false,                  // inverse
            false,                  // filtr
			0                       // addShiftADC
        },
        {
            1.0F,                   // коэффициент калибровки
            RShiftZero,
            {{0}, {0}},             // rShiftAdd
            ModeCouple::AC,          // ModCouple
            Divider::_1,           // Divider
            Range::_500mV,            // range
            true,                   // enable
            false,                  // inverse
            false,                  // filtr
			5                       // addShiftADC
        }
        
    },
    // trig
    {
        StartMode::Auto,
        TrigSource::A,
        TrigPolarity::Front,
        TrigInput::Full,
        {TrigLevZero, TrigLevZero, TrigLevZero},
        ModeLongPressTrig::Auto,
        TrigModeFind::Hand
    },
    // time
    {
        TBase::_200us,
        0,                          // set.time.tShiftRel
        FunctionTime::Time,
        TPos::Center,
        SampleType::Equal,
        PeackDetMode::Disable,
        false,
        ENUM_POINTS_FPGA::_1024
    },
    // cursors
    {
        { CursCntrl::Disable, CursCntrl::Disable },   // CursCntrl U
        { CursCntrl::Disable, CursCntrl::Disable },   // CursCntrl T
        Channel::A,                                      // source
        { 60.0F,  140.0F, 60.0F, 140.0F },          // posCur U
        { 80.0F,  200.0F, 80.0F, 200.0F },          // posCur T
        { 80.0F,  80.0F },                          // расстояние между курсорами напряжения для 100%
        { 120.0F, 120.0F },                         // расстояние между курсорами времени для 100%
        CursMovement::Points,                        // CursMovement
        CursActive::None,                            // CursActive
        { CursLookMode::None, CursLookMode::None },   // Режим слежения курсоров.
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
        ModeShowIntMem::Both,       // modeShowIntMem
        false,                      // flashAutoConnect
        ModeBtnMemory::Menu,
        ModeSaveSignal::BMP
    },
    // measures
    {
        MeasuresNumber::_1,         // measures number
        Channel::A_B,               // source
        ModeViewSignals::AsIs,      // mode view signal
        {
            Measure::VoltageMax,     Measure::VoltageMin,     Measure::VoltagePic,         Measure::VoltageMaxSteady,   Measure::VoltageMinSteady,
            Measure::VoltageAverage, Measure::VoltageAmpl,    Measure::VoltageRMS,         Measure::VoltageVybrosPlus,  Measure::VoltageVybrosMinus,
            Measure::Period,         Measure::Freq,           Measure::TimeNarastaniya,    Measure::TimeSpada,          Measure::DurationPlus
        },
        false,                      // show
        MeasuresField::Screen,      // брать для измерений значения, видимые на экране
        //{0, 200},                 // начальные значения курсоров напряжения для зоны измерений
        //{372, 652},               // начальные значения курсоров времени для зоны измерений
        {50, 150},                  // начальные значения курсоров напряжения для зоны измерений
        {422, 602},                 // начальные значения курсоров времени для зоны измерений
        CursCntrl::_1,              // Управляем первым курсором напряжения
        CursCntrl::_1,              // Управляем первым курсором времени
        CursActive::T,              // Активны курсоры времени.
        Measure::None                // marked Measure
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
        RShiftZero                  // rShift
    },
    // service
    {
        true,                       // screenWelcomeEnable
        false,                      // soundEnable
        CalibratorMode::Freq,       // calibratorEnable
        0,                          // IP-адрес (временно)  WARN
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
    // menu
    {
        true,
        1
    },
    // debug
    {
        30,                          // numStrings
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
        0,                          // pretriggered
        {
            false,                  // показывать ли флаг готовности
            false,                  // rShift0
            false,                  // rShift1
            false                   // trigLev
        }
    }
};

Settings set;


void Settings::Load(bool /*_default*/)
{
    set = defaultSettings;

//    if (!_default)
//    {
//        _default = !EPROM::LoadSettings();
//        
//    }
//    
//    if(_default)
//    {
//        uint16 rShiftAddA[Range::Count][2];
//        uint16 rshiftAddB[Range::Count][2];
//
//        std::memcpy((void *)rShiftAddA, (void *)&RSHIFT_ADD(Channel::A, 0, 0), 2 * Range::Count * 2); // Сначала сохраняем несбрасываемые настройки
//        std::memcpy((void *)rshiftAddB, (void *)&RSHIFT_ADD(Channel::B, 0, 0), 2 * Range::Count * 2);
//
//        int16  balanceADC0 = BALANCE_ADC_A;
//        int16  balanceADC1 = BALANCE_ADC_B;
//        int16  numAverageForRand = NUM_AVE_FOR_RAND;
//        BalanceADCtype::E balanceType = BALANCE_ADC_TYPE;
//
//        std::memcpy((void*)&set, (void*)(&defaultSettings), sizeof(set));                // Потом заполняем значениями по умолчанию
//
//        std::memcpy((void *)&RSHIFT_ADD(Channel::A, 0, 0), (void *)rShiftAddA, 2 * Range::Count * 2);  // И восстанавливаем несбрасываемые настройки
//        std::memcpy((void *)&RSHIFT_ADD(Channel::B, 0, 0), (void *)rshiftAddB, 2 * Range::Count * 2);
//
//        BALANCE_ADC_A = balanceADC0;
//        BALANCE_ADC_B = balanceADC1;
//        NUM_AVE_FOR_RAND = numAverageForRand;
//        BALANCE_ADC_TYPE = balanceType;
//    }
//
//    Panel::EnableLEDChannel0(sChannel_Enabled(Channel::A));
//    Panel::EnableLEDChannel1(sChannel_Enabled(Channel::B));
//    Menu::SetAutoHide(true);
//    Display::ChangedRShiftMarkers();
//
//    Settings::loaded = true;
}

void Settings::Save()
{
    EPROM::SaveSettings();
}

bool Settings::DebugModeEnable()
{
    return true;
}

Range::E& operator++(Range::E &range)
{
    range = (Range::E)((int)range + 1);
    return range;

}


Range::E& operator--(Range::E &range)
{
    range = (Range::E)((int)range - 1);
    return range;
}
