#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/EPROM.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstdio>
#include <cstdlib>


using namespace Primitives;


extern const Page pDebug;


extern const Choice     mcStats;                            // ОТЛАДКА - Статистика
extern const Page       mpConsole;                          // ОТЛАДКА - КОНСОЛЬ
extern const Governor   mgConsole_NumStrings;               // ОТЛАДКА - КОНСОЛЬ - Число строк
extern const Choice     mcConsole_SizeFont;                 // ОТЛАДКА - КОНСОЛЬ - Размер шрифта
extern const Page       mpConsole_Registers;                // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ
extern const Choice     mcConsole_Registers_ShowAll;        // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все
extern const Choice     mcConsole_Registers_RD_FL;          // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL
static bool      IsActive_Console_Registers_RD_FL();
extern const Choice     mcConsole_Registers_RShiftA;        // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к
extern const Choice     mcConsole_Registers_RShiftB;        // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к
extern const Choice     mcConsole_Registers_TrigLev;        // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр.
extern const Choice     mcConsole_Registers_RangeA;         // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1
extern const Choice     mcConsole_Registers_RangeB;         // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2
extern const Choice     mcConsole_Registers_TrigParam;      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр.
extern const Choice     mcConsole_Registers_ChanParamA;     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 1
extern const Choice     mcConsole_Registers_ChanParamB;     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2
extern const Choice     mcConsole_Registers_TBase;          // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ
extern const Choice     mcConsole_Registers_tShift;         // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см.
extern const Page       mpADC;                              // ОТЛАДКА - АЦП
extern const Page       mpADC_Balance;                      // ОТЛАДКА - АЦП - БАЛАНС
extern const Choice     mcADC_Balance_Mode;                 // ОТЛАДКА - АЦП - БАЛАНС - Режим
static void     OnChanged_ADC_Balance_Mode(bool active);
static void          Draw_ADC_Balance_Mode(int x, int y);
extern const Governor   mgADC_Balance_ShiftA;               // ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
static bool      IsActive_ADC_Balance_Shift();
static void     OnChanged_ADC_Balance_ShiftA();
extern const Governor   mgADC_Balance_ShiftB;               // ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
static void     OnChanged_ADC_Balance_ShiftB();
extern const Page       mpADC_Stretch;                      // ОТЛАДКА - АЦП - РАСТЯЖКА
extern const Choice     mcADC_Stretch_Mode;                 // ОТЛАДКА - АЦП - РАСТЯЖКА - Режим
static void     OnChanged_ADC_Stretch_Mode(bool active);
void LoadStretchADC(Channel::E chan);
extern const Governor   mgADC_Stretch_ADC_A;                // ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 1к
static bool      IsActive_ADC_Stretch_ADC();
static void     OnChanged_ADC_Stretch_ADC_A();
extern const Governor   mgADC_Stretch_ADC_B;                // ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 2к
static void     OnChanged_ADC_Stretch_ADC_B();
extern const Page       mpADC_AltRShift;                    // ОТЛАДКА - АЦП - ДОП СМЕЩ
extern const Button     mbADC_AltRShift_Reset;              // ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс
static void       OnPress_ADC_AltRShift_Reset();
extern const Governor   mbADC_AltRShift_2mV_DC_A;           // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост
static void     OnChanged_ADC_AltRShift_A();
extern const Governor   mbADC_AltRShift_2mV_DC_B;           // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост
static void     OnChanged_ADC_AltRShift_B();
extern const Governor   mbADC_AltRShift_5mV_DC_A;           // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост
extern const Governor   mbADC_AltRShift_5mV_DC_B;           // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост
extern const Governor   mbADC_AltRShift_10mV_DC_A;          // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост
extern const Governor   mbADC_AltRShift_10mV_DC_B;          // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост
extern const Page       mpRandomizer;                       // ОТЛАДКА - РАНД-ТОР
extern const Governor   mgRandomizer_SamplesForGates;       // ОТЛАДКА - РАНД-ТОР - Выб-к/ворота
static void     OnChanged_Randomizer_SamplesForGates();
extern const Governor   mgRandomizer_AltTShift0;            // ОТЛАДКА - РАНД-ТОР - tShift доп.
static void     OnChanged_Randomizer_AltTShift0();
extern const Governor   mgRandomizer_Average;               // ОТЛАДКА - РАНД-ТОР - Усредн.
extern const Choice     mcSizeSettings;                     // ОТЛАДКА - Размер настроек
static void        OnDraw_SizeSettings(int x, int y);
extern const Button     mbSaveFirmware;                     // ОТЛАДКА - Сохр. прошивку
static bool      IsActive_SaveFirmware();
static void       OnPress_SaveFirmware();
extern const       Page ppSerialNumber;                     // ОТЛАДКА - С/Н
static void       OnPress_SerialNumber();
static void Draw_EnterSerialNumber();
static void      OnRegSet_SerialNumber(int);
extern const SmallButton bSerialNumber_Exit;                // ОТЛАДКА - С/Н - Выход
static void       OnPress_SerialNumber_Exit();
extern const SmallButton bSerialNumber_Change;              // ОТЛАДКА - С/Н - Перейти
static void       OnPress_SerialNumber_Change();
static void          Draw_SerialNumber_Change(int, int);
extern const  SmallButton bSerialNumber_Save;               // ОТЛАДКА - С/Н - Сохранить
static void        OnPress_SerialNumber_Save();
static void           Draw_SerialNumber_Save(int, int);
extern const       Button bEraseData;                       // ОТЛАДКА - Стереть данные
static void       OnPress_EraseData();


// В этой структуре будут храниться данные серийного номера при открытой странице ppSerialNumer
struct StructForSN
{
    int number;     // Соответственно, порядковый номер.
    int year;       // Соответственно, год.
    int curDigt;    // Соответственно, номером (0) или годом (1) управляет ручка УСТАНОВКА.
};


const Page *PageDebug::SerialNumber::self = &ppSerialNumber;


// ОТЛАДКА /////////////////////////
DEF_PAGE_7(pDebug, PageMain::self, NamePage::Debug,
    "ОТЛАДКА", "DEBUG",
    "",
    "",
    mcStats,            // ОТЛАДКА - Статистика
    mpConsole,          // ОТЛАДКА - КОНСОЛЬ
    mpADC,              // ОТЛАДКА - АЦП
    mpRandomizer,       // ОТЛАДКА - РАНД-ТОР
    mcSizeSettings,     // ОТЛАДКА - Размер настроек
    mbSaveFirmware,     // ОТЛАДКА - Сохр. прошивку
    bEraseData,         // ОТЛАДКА - Стереть данные
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::self = &pDebug;


// ОТЛАДКА - Статистика ------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcStats, &pDebug,
    "Статистика", "Statistics",
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в памяти/количество сохраняемых в памяти сигналов",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "Не показывать", "Hide",
    "Показывать",    "Show",
    SHOW_STATS, nullptr, nullptr, nullptr
)


// ОТЛАДКА - КОНСОЛЬ ///////////////
DEF_PAGE_3(mpConsole, pDebug, NamePage::DebugConsole,
    "КОНСОЛЬ", "CONSOLE",
    "",
    "",
    mgConsole_NumStrings,   // ОТЛАДКА - КОНСОЛЬ - Число строк
    mcConsole_SizeFont,     // ОТЛАДКА - КОНСОЛЬ - Размер шрифта
    mpConsole_Registers,    // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ
    nullptr, nullptr, nullptr, nullptr
);

// ОТЛАДКА - КОНСОЛЬ - Число строк -------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgConsole_NumStrings, mpConsole,
    "Число строк", "Number strings",
    "",
    "",
    NUM_STRINGS,  0, 33, nullptr, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - Размер шрифта -----------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_SizeFont, &mpConsole,
    "Размер шрифта", "Size font",
     "",
     "",
     "5", "5",
     "8", "8",
    SIZE_FONT_CONSOLE, nullptr, nullptr, nullptr
)


// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ ////
DEF_PAGE_12(mpConsole_Registers, mpConsole, NamePage::DebugShowRegisters,
    "РЕГИСТРЫ", "REGISTERS",
    "",
    "",
    mcConsole_Registers_ShowAll,    // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все
    mcConsole_Registers_RD_FL,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL
    mcConsole_Registers_RShiftA,    // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к
    mcConsole_Registers_RShiftB,    // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к
    mcConsole_Registers_TrigLev,    // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр.
    mcConsole_Registers_RangeA,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1
    mcConsole_Registers_RangeB,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2
    mcConsole_Registers_TrigParam,  // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр.
    mcConsole_Registers_ChanParamA, // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 1
    mcConsole_Registers_ChanParamB, // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2
    mcConsole_Registers_TBase,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ
    mcConsole_Registers_tShift,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см.
    nullptr, nullptr, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все -----------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_ShowAll, &mpConsole_Registers,
    "Показывать все", "Show all",
    "Показывать все значения, засылаемые в регистры",
    "To show all values transferred in registers",
    "Нет", "No",
    "Да",  "Yes",
    IS_SHOW_REGISTERS_ALL, nullptr, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL --------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RD_FL, &mpConsole_Registers, 
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.flag, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

static bool IsActive_Console_Registers_RD_FL(void)
{
    return !IS_SHOW_REGISTERS_ALL;
}

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RShiftA, &mpConsole_Registers,
    "U см. 1к", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.rShiftA, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RShiftB, &mpConsole_Registers,
    "U см. 2к", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.rShiftB, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр. -----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_TrigLev, &mpConsole_Registers,
    "U синхр.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.trigLev, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1 --------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RangeA, &mpConsole_Registers,
    "ВОЛЬТ/ДЕЛ 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.range[0], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2 --------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_RangeB, &mpConsole_Registers,
    "ВОЛЬТ/ДЕЛ 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.range[1], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр. ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_TrigParam, &mpConsole_Registers,
    "Парам. синхр.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.trigParam, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 1 ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_ChanParamA, &mpConsole_Registers,
    "Парам. кан. 1",  "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.chanParam[0], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_ChanParamB, &mpConsole_Registers,
    "Парам. кан. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.chanParam[1], IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ ----------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_TBase, &mpConsole_Registers,
    "ВРЕМЯ/ДЕЛ", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.tBase, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)

// ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см. --------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(mcConsole_Registers_tShift, &mpConsole_Registers,
    "Т см.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.debug.showRegisters.tShift, IsActive_Console_Registers_RD_FL, nullptr, nullptr
)


// ОТЛАДКА - АЦП ///////////////////
DEF_PAGE_3(mpADC, pDebug, NamePage::DebugADC,
    "АЦП", "ADC",
    "",
    "",
    mpADC_Balance,      // ОТЛАДКА - АЦП - БАЛАНС
    mpADC_Stretch,      // ОТЛАДКА - АЦП - РАСТЯЖКА
    mpADC_AltRShift,    // ОТЛАДКА - АЦП - ДОП СМЕЩ
    nullptr, nullptr, nullptr, nullptr
)

// ОТЛАДКА - АЦП - БАЛАНС //////////
DEF_PAGE_3(mpADC_Balance, mpADC, NamePage::DebugADCbalance,
    "БАЛАНС", "BALANCE",
    "",
    "",
    mcADC_Balance_Mode,     // ОТЛАДКА - АЦП - БАЛАНС - Режим
    mgADC_Balance_ShiftA,   // ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
    mgADC_Balance_ShiftB,   // ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
    nullptr, nullptr, nullptr, nullptr
)

// ОТЛАДКА - АЦП - БАЛАНС - Режим --------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcADC_Balance_Mode, &mpADC_Balance,
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной",   "Manual",
    BALANCE_ADC_TYPE, nullptr, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)

static int16 shiftADCA;
static int16 shiftADCB;

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);

    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);
}

static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, static_cast<int8>(BALANCE_ADC_A)},
        {0, SET_BALANCE_ADC_B, static_cast<int8>(BALANCE_ADC_B)}
    };

    shiftADCA = shift[0][BALANCE_ADC_TYPE];
    shiftADCB = shift[1][BALANCE_ADC_TYPE];
}

// ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Balance_ShiftA, mpADC_Balance, 
    "Смещение 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftA, nullptr
)

static void OnChanged_ADC_Balance_ShiftA(void)
{
    BALANCE_ADC_A = shiftADCA;
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
}

static bool IsActive_ADC_Balance_Shift(void)
{
    return BALANCE_ADC_TYPE_IS_HAND;
}

// ОТЛАДКА - АЦП - БАЛАНС - Смещение 2----------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Balance_ShiftB, mpADC_Balance,
    "Смещение 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftB, nullptr
)

static void OnChanged_ADC_Balance_ShiftB(void)
{
    BALANCE_ADC_B = shiftADCB;
    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
}


// ОТЛАДКА - АЦП - РАСТЯЖКА ////////
DEF_PAGE_3(mpADC_Stretch, mpADC, NamePage::DebugADCstretch,
    "РАСТЯЖКА", "STRETCH",
    "",
    "",
    mcADC_Stretch_Mode,     // ОТЛАДКА - АЦП - РАСТЯЖКА - Режим
    mgADC_Stretch_ADC_A,    // ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 1к
    mgADC_Stretch_ADC_B,    // ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 2к    
    nullptr, nullptr, nullptr, nullptr
)

// ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(mcADC_Stretch_Mode, &mpADC_Stretch,
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной",   "Manual",
    DEBUG_STRETCH_ADC_TYPE, nullptr, OnChanged_ADC_Stretch_Mode, nullptr
)

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if(active)
    {
        LoadStretchADC(Channel::A);
        LoadStretchADC(Channel::B);
    }
}

void LoadStretchADC(Channel::E chan)
{
    if(DEBUG_STRETCH_ADC_TYPE_IS_DISABLED)
    {
        FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if(DEBUG_STRETCH_ADC_TYPE_IS_HAND)
    {
        FPGA::WriteToHardware(chan == Channel::A ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(chan), true);
    }
    else if(DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS)
    {
        FPGA::LoadKoeffCalibration(chan);
    }
}

// ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 1к --------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Stretch_ADC_A, mpADC_Stretch,
    "Коэфф. 1к", "Koeff. 1ch",
    "",
    "",
    DEBUG_STRETCH_ADC_A, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_A, nullptr
)

static bool IsActive_ADC_Stretch_ADC(void)
{
    return DEBUG_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_ADC_A(void)
{
    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}


// ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 2к --------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgADC_Stretch_ADC_B, mpADC_Stretch,
    "Коэфф. 2к", "Koeff. 2ch",
    "",
    "",
    DEBUG_STRETCH_ADC_B, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_B, nullptr
)

static void OnChanged_ADC_Stretch_ADC_B(void)
{
    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

// ОТЛАДКА - АЦП - ДОП СМЕЩ ////////
DEF_PAGE_7(mpADC_AltRShift, &mpADC, NamePage::DebugADCrShift,
    "ДОП СМЕЩ", "ADD RSHFIT",
    "",
    "",
    mbADC_AltRShift_Reset,          // ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс
    mbADC_AltRShift_2mV_DC_A,       // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост
    mbADC_AltRShift_2mV_DC_B,       // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост
    mbADC_AltRShift_5mV_DC_A,       // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост
    mbADC_AltRShift_5mV_DC_B,       // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост
    mbADC_AltRShift_10mV_DC_A,      // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост
    mbADC_AltRShift_10mV_DC_B,      // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост
    nullptr, nullptr, nullptr, nullptr
)

// ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс ------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(mbADC_AltRShift_Reset, mpADC_AltRShift,
    "Сброс", "Reset",
    "", "",
    nullptr, OnPress_ADC_AltRShift_Reset
)

static void OnPress_ADC_AltRShift_Reset(void)
{
    for(int chan = 0; chan < 2; chan++)
    {
        for(int mode = 0; mode < 2; mode++)
        {
            for(int range = 0; range < Range::Count; range++)
            {
                RSHIFT_ADD(chan, range, mode) = 0;
            }
        }
    }
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}

// ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_A, mpADC_AltRShift,
    "См 1к 2мВ пост", "Shift 1ch 2mV DC",
    "",
    "",
    set.chan[Channel::A].rShiftAdd[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

static void OnChanged_ADC_AltRShift_A(void)
{
    FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
}

// ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_B, mpADC_AltRShift,
    "См 2к 2мВ пост", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

static void OnChanged_ADC_AltRShift_B(void)
{
    FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
}

// ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_A, mpADC_AltRShift,
    "См 1к 5мВ пост", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::A, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

// ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост ---------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_B, mpADC_AltRShift,
    "См 2к 5мВ пост", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

// ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост --------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_A, mpADC_AltRShift,
    "См 1к 10мВ пост", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::A, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

// ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост --------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_B, mpADC_AltRShift,
    "См 2к 10мВ пост", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(Channel::B, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)


// ОТЛАДКА - РАНД-ТОР //////////////
DEF_PAGE_3(mpRandomizer, pDebug, NamePage::DebugRandomizer,
    "РАНД-ТОР", "RANDOMIZER",
    "",
    "",
    mgRandomizer_SamplesForGates,   // ОТЛАДКА - РАНД-ТОР - Выб-к/ворота
    mgRandomizer_AltTShift0,        // ОТЛАДКА - РАНД-ТОР - tShift доп.
    mgRandomizer_Average,           // ОТЛАДКА - РАНД-ТОР - Усредн.
    nullptr, nullptr, nullptr, nullptr
)

// ОТЛАДКА - РАНД-ТОР - Выб-к/ворота -----------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgRandomizer_SamplesForGates, mpRandomizer,
    "Выб-к/ворота", "Samples/gates",
    "",
    "",
    NUM_MEAS_FOR_GATES, 1, 2500, nullptr, OnChanged_Randomizer_SamplesForGates, nullptr
)

static void OnChanged_Randomizer_SamplesForGates(void)
{
    FPGA::SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
}

// ОТЛАДКА - РАНД-ТОР - tShift доп. ------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgRandomizer_AltTShift0, mpRandomizer,
    "tShift доп.", "tShift alt.",
    "",
    "",
    ADD_SHIFT_T0, 0, 510, nullptr, OnChanged_Randomizer_AltTShift0, nullptr
)

static void OnChanged_Randomizer_AltTShift0(void)
{
    FPGA::SetDeltaTShift(ADD_SHIFT_T0);
}

// ОТЛАДКА - РАНД-ТОР - Усредн. ----------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(mgRandomizer_Average, mpRandomizer,
    "Усредн.", "Average",
    "",
    "",
    NUM_AVE_FOR_RAND, 1, 32, nullptr, nullptr, nullptr
)


// ОТЛАДКА - Размер настроек  ------------------------------------------------------------------------------------------------------------------------
static int8 size = 0;
DEF_CHOICE_2(mcSizeSettings, &pDebug,
    "Размер настроек", "Size settings",
    "Вывод размера структуры Settings",
    "Show size of struct Settings",
    "Размер", "Size",
    "Размер", "Size",
    size, nullptr, nullptr, OnDraw_SizeSettings
)

static void OnDraw_SizeSettings(int x, int y)
{
    Painter::DrawFormatText(x + 5, y + 21, Color::BLACK, "Размер %d", sizeof(Settings));
}


// ОТЛАДКА - Сохр. прошивку --------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(mbSaveFirmware, pDebug,
    "Сохр. прошивку", "Save firmware",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)

static bool IsActive_SaveFirmware()
{
    return FLASH_DRIVE_IS_CONNECTED == 1;
}

static void OnPress_SaveFirmware()
{
    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-53.bin", &structForWrite);

    uint8 *address = (uint8*)0x08020000; //-V566
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::ShowWarningGood(Warning::FirmwareSaved);
}


DEF_BUTTON(bEraseData, pDebug,
    "Стереть данные", "Erase data",
    "Стирает сектора с данными",
    "Erases data sectors",
    nullptr, OnPress_EraseData
)

static void OnPress_EraseData()
{
    EPROM::EraseData();
}


// ОТЛАДКА - С/Н ///////////////////
DEF_PAGE_6(ppSerialNumber, &pDebug, NamePage::SB_SerialNumber,
    "С/Н", "S/N",
    "Запись серийного номера в OTP-память. ВНИМАНИЕ!!! ОТP-память - память с однократной записью.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    bSerialNumber_Exit,     // ОТЛАДКА - С/Н - Выход
    bSerialNumber_Change,   // ОТЛАДКА - С/Н - Перейти
    Item::empty,
    Item::empty,
    Item::empty,
    bSerialNumber_Save,      // ОТЛАДКА - С/Н - Сохранить    
    nullptr, OnPress_SerialNumber, nullptr, OnRegSet_SerialNumber
)

static void OnPress_SerialNumber(void)
{
    PageDebug::SerialNumber::self->OpenAndSetItCurrent();
    Display::SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 1; //-V522
    s->year = 2017;
    s->curDigt = 0;
}

static void Draw_EnterSerialNumber(void)
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Primitives::Rectangle(width, height).Draw(x0, y0, COLOR_FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, COLOR_BACK);

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);

    bool selNumber = s->curDigt == 0;

    char buffer[20];
    std::snprintf(buffer, 19, "%02d", s->number);

    Color::E colorText = COLOR_FILL;
    Color::E colorBackground = COLOR_BACK;

    if (selNumber)
    {
        colorText = Color::FLASH_01;
        colorBackground = Color::FLASH_10;
    }

    int y = y0 + 50;

    Color::SetCurrent(colorText);
    int x = Painter::DrawTextOnBackground(x0 + deltaX, y, buffer, colorBackground);

    colorText = Color::FLASH_01;
    colorBackground = Color::FLASH_10;

    if (selNumber)
    {
        colorText = COLOR_FILL;
        colorBackground = COLOR_BACK;
    }

    std::snprintf(buffer, 19, "%04d", s->year);

    Color::SetCurrent(colorText);
    Painter::DrawTextOnBackground(x + 5, y, buffer, colorBackground);

    // Теперь выведем информацию об оставшемся месте в OTP-памяти для записи

    int allShots = HAL_OTP::GetSerialNumber(buffer);

    Painter::DrawFormatText(x0 + deltaX, y0 + 130, COLOR_FILL, "Текущий сохранённый номер %s", buffer[0] == 0 ? "-- ----" : buffer);

    Painter::DrawFormatText(x0 + deltaX, y0 + 100, COLOR_FILL, "Осталось места для %d попыток", allShots);
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int(*pFunc)(int*, int, int);

    pFunc p = angle > 0 ? CircleIncreaseInt : CircleDecreaseInt;

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        p(&s->number, 1, 99);
    }
    else
    {
        p(&s->year, 2014, 2050);
    }
    Sound::GovernorChangedValue();
}

// ОТЛАДКА - С/Н - Выход -----------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(bSerialNumber_Exit, ppSerialNumber,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_SerialNumber_Exit, DrawSB_Exit, nullptr
)

static void OnPress_SerialNumber_Exit(void)
{
    Display::RemoveAddDrawFunction();
    FREE_EXTRAMEM();
}

// ОТЛАДКА - С/Н - Вставить --------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(bSerialNumber_Change, ppSerialNumber,
    "Вставить", "Insert",
    "Вставляет выбраный символ",
    "Inserts the chosen symbol",
    nullptr, OnPress_SerialNumber_Change, Draw_SerialNumber_Change, nullptr
)

static void OnPress_SerialNumber_Change(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);
    ++s->curDigt;
    s->curDigt %= 2;
    Painter::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_TAB);
    Font::Set(TypeFont::_8);
}

// ОТЛАДКА - С/Н - Сохранить -------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(bSerialNumber_Save, ppSerialNumber,
    "Сохранить", "Save",
    "Записывает серийный номер в OTP",
    "Records the serial number in OTP",
    nullptr, OnPress_SerialNumber_Save, Draw_SerialNumber_Save, nullptr
)

static void OnPress_SerialNumber_Save(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);

    char stringSN[20];

    std::snprintf(stringSN, 19, "%02d %04d", s->number, s->year);

    if (!HAL_OTP::SaveSerialNumber(stringSN))
    {
        Display::ShowWarningBad(Warning::FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Font::Set(TypeFont::_8);
}



/** @}  @}
 */

        /*
        static void OnChangeRShiftADC(bool active)
        {
            FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
            FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
        }

        const Choice mcDebugADCrShiftMode =
        {
            TypeItem::Choice, &mpADC_AltRShift, {"Режим",      "Mode"},
            {
                "",
                ""
            },
            EmptyFuncBV,
            {                                    {DISABLE_RU,   DISABLE_EN},
                                                 {"Реальный",   "Real"},
                                                 {"Ручной",     "Hand"}
            },
            (int8*)&set.debug.rShiftADCtype, OnChangeRShiftADC
        };

        // ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ - Смещение 1к 2мВ
        static TimeStructGovernor tsRShiftADC;
        static bool IsActiveRShiftADC(void)
        {
            return set.debug.rShiftADCtype == RShiftADC_Hand;
        }

        static void OnChangeRShiftADC0(void)
        {
            FPGA::SetRShift(Channel::A, SET_RSHIFT_A);
        }

        const Governor mgDebugRShiftADC00 =
        {
            &mpADC_AltRShift,
            {
                "Смещ 1к 2мВ", "Shift 1ch"
            },
            {
                "",
                ""
            },
            IsActiveRShiftADC,
            &set.debug.rShiftADC[0][0], -100, 100, &tsRShiftADC, OnChangeRShiftADC0
        };

        static TimeStructGovernor tsRShiftADC01;
        const Governor mgDebugRShiftADC01 =
        {
            &mpADC_AltRShift,
            {
                "Смещ 1к 500мВ", "Shift 1ch 500mV"
            },
            {
                "",
                ""
            },
            IsActiveRShiftADC,
            &set.debug.rShiftADC[1][0], -100, 100, &tsRShiftADC01, OnChangeRShiftADC0
        };

        static void OnChangeRShiftADC1(void)
        {
            FPGA::SetRShift(Channel::B, SET_RSHIFT_B);
        }

        // ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
const Page mspDebugADCaltShift;

// ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ. - Величина
const Governor mgDebugADCaltShift =
{
    &mspDebugADCaltShift, 0,
    {
        "Величина", "Value",
        "",
        ""
    },
    &set.debug.altShift, -2, 2
};

// ОТЛАДКА - АЦП - ДОП СМЕЩ ПАМ //////
const Page mspDebugADCaltShift =
{
    TypeItem::Page, &mpADC, 0,
    {
        "ДОП СМЕЩ ПАМ", "ALT SHIFT MEM",
        "",
        ""
    },
    NamePage::DebugADCaltShift,
    {
        (void*)&mgDebugADCaltShift
    }
};

const Governor mgDebugInfoNumber =
{
    &mspDebugInformation,
    {
        "Номер", "Number"
    },
    {
        "Установка номера серийного номера",
        "Installation numbers of the serial number"
    },
    EmptyFuncBV,
    &set.debug.infoNumber, 0, 100
};

const Governor mgDebugInfoYear =
{
    &mspDebugInformation,
    {
        "Год", "Year"
    },
    {
        "Установка года серийного номера",
        "Installation year serial number"
    },
    EmptyFuncBV,
    &set.debug.infoYear, 2015, 2050
};

const Governor mgDebugInfoVersion =
{
    &mspDebugInformation,
    {
        "Версия", "Version"
    },
    {
        "Установка номера версии",
        "Setting the version number"
    },
    EmptyFuncBV,
    &set.debug.infoVer, 10, 100
};

// ОТЛАДКА - ИНФОРМАЦИЯ /////////////////////
const Page mspDebugInformation =
{
    TypeItem::Page, &pDebug,
    {
        "ИНФОРМАЦИЯ", "INFORMATION",
    },
    {
        "Позволяет ввести информацию для меню СЕРВИС-ИНФОРМАЦИЯ",
        "It allows you to enter information for SERVICE INFORMATION menu"
    },
    EmptyFuncBV, NamePage::DebugInformation,
    {
        (void*)&mgDebugInfoNumber,
        (void*)&mgDebugInfoYear,
        (void*)&mgDebugInfoVersion
    }
};
*/
