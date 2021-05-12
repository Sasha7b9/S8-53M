// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstdlib>


static int16 shiftADCA;
static int16 shiftADCB;
static int8 size = 0;


// В этой структуре будут храниться данные серийного номера при открытой странице ppSerialNumer
struct StructForSN
{
    int number;     // Соответственно, порядковый номер.
    int year;       // Соответственно, год.
    int curDigt;    // Соответственно, номером (0) или годом (1) управляет ручка УСТАНОВКА.
};


DEF_CHOICE_2(mcStats, PageDebug::self,
    "Статистика", "Statistics",
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в памяти/количество сохраняемых в памяти сигналов",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "Не показывать", "Hide",
    "Показывать", "Show",
    setNRST.show_stats, nullptr, nullptr, nullptr
)


static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, set.chan[ChA].balance_shift_ADC, (int8)setNRST.balance_ADC[ChA]},
        {0, set.chan[ChB].balance_shift_ADC, (int8)setNRST.balance_ADC[ChB]}
    };

    shiftADCA = shift[0][BalanceADCtype::Get()];
    shiftADCB = shift[1][BalanceADCtype::Get()];
}

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);

//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);
}

DEF_CHOICE_3(mcADC_Balance_Mode, PageDebug::PageADC::PageBalance::self,
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной", "Manual",
    setNRST.balance_ADC_type, nullptr, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)

static void OnChanged_ADC_Balance_ShiftA()
{
    setNRST.balance_ADC[ChA] = shiftADCA;
//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
}

static bool IsActive_ADC_Balance_Shift()
{
    return (BalanceADCtype::Get() == BalanceADCtype::Hand);
}

DEF_GOVERNOR(mgADC_Balance_ShiftA, PageDebug::PageADC::PageBalance::self,
    "Смещение 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftA, nullptr
)

static void OnChanged_ADC_Balance_ShiftB()
{
    setNRST.balance_ADC[ChB] = shiftADCB;
//    FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
}

DEF_GOVERNOR(mgADC_Balance_ShiftB, PageDebug::PageADC::PageBalance::self,
    "Смещение 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftB, nullptr
)

DEF_PAGE_3(pageBalanceADC, PageDebug::PageADC::self, NamePage::DebugADCbalance,
    "БАЛАНС", "BALANCE",
    "",
    "",
    mcADC_Balance_Mode,     // ОТЛАДКА - АЦП - БАЛАНС - Режим
    mgADC_Balance_ShiftA,   // ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
    mgADC_Balance_ShiftB,   // ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
    nullptr, nullptr, nullptr, nullptr
)

void PageDebug::LoadStretchADC(const Channel &ch)
{
    if (StretchADCtype::IsDisabled())
    {
//        FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if (StretchADCtype::IsHand())
    {
//        FPGA::WriteToHardware(ch == ChA ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(ch), true);
    }
    else if (StretchADCtype::IsSettings())
    {
        FPGA::Calibrator::LoadKoeff(ch);
    }
}

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if (active)
    {
        PageDebug::LoadStretchADC(ChA);
        PageDebug::LoadStretchADC(ChB);
    }
}

DEF_CHOICE_3(mcADC_Stretch_Mode, PageDebug::PageADC::PageStretch::self,
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной", "Manual",
    setNRST.stretch_ADC_type, nullptr, OnChanged_ADC_Stretch_Mode, nullptr
)

static void OnChanged_ADC_Stretch_ADC_A()
{
//    FPGA::WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}

static bool IsActive_ADC_Stretch_ADC()
{
    return StretchADCtype::IsHand();
}

DEF_GOVERNOR(mgADC_Stretch_ADC_A, PageDebug::PageADC::PageStretch::self,
    "Коэфф. 1к", "Koeff. 1ch",
    "",
    "",
    setNRST.stretch_ADC[ChA], 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_A, nullptr
)

static void OnChanged_ADC_Stretch_ADC_B()
{
//    FPGA::WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

DEF_GOVERNOR(mgADC_Stretch_ADC_B, PageDebug::PageADC::PageStretch::self,
    "Коэфф. 2к", "Koeff. 2ch",
    "",
    "",
    setNRST.stretch_ADC[ChB], 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_B, nullptr
)

DEF_PAGE_3(pageStretchADC, PageDebug::PageADC::self, NamePage::DebugADCstretch,
    "РАСТЯЖКА", "STRETCH",
    "",
    "",
    mcADC_Stretch_Mode,     // ОТЛАДКА - АЦП - РАСТЯЖКА - Режим
    mgADC_Stretch_ADC_A,    // ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 1к
    mgADC_Stretch_ADC_B,    // ОТЛАДКА - АЦП - РАСТЯЖКА - Коэфф. 2к    
    nullptr, nullptr, nullptr, nullptr
)

static void OnPress_ADC_AltRShift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < Range::Count; range++)
            {
                RSHIFT_ADD(ch, range, mode) = 0;
            }
        }
    }
    RShift::Set(ChA, set.chan[ChA].rshift);
    RShift::Set(ChB, set.chan[ChB].rshift);
}

DEF_BUTTON(mbADC_AltRShift_Reset, PageDebug::PageADC::PageAltRShift::self,
    "Сброс", "Reset",
    "", "",
    nullptr, OnPress_ADC_AltRShift_Reset
)

static void OnChanged_ADC_AltRShift_A()
{
    RShift::Set(ChA, set.chan[ChA].rshift);
}

DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "См 1к 2мВ пост", "Shift 1ch 2mV DC",
    "",
    "",
    set.chan[ChA].rshift_add[Range::_2mV][ModeCouple::DC], -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

static void OnChanged_ADC_AltRShift_B()
{
    RShift::Set(ChB, set.chan[ChB].rshift);
}

DEF_GOVERNOR(mbADC_AltRShift_2mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "См 2к 2мВ пост", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_2mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "См 1к 5мВ пост", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(ChA, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_5mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "См 2к 5мВ пост", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_5mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_A, PageDebug::PageADC::PageAltRShift::self,
    "См 1к 10мВ пост", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(ChA, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_A, nullptr
)

DEF_GOVERNOR(mbADC_AltRShift_10mV_DC_B, PageDebug::PageADC::PageAltRShift::self,
    "См 2к 10мВ пост", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD(ChB, Range::_10mV, ModeCouple::DC), -100, 100, nullptr, OnChanged_ADC_AltRShift_B, nullptr
)

DEF_PAGE_7(pageAltShiftADC, PageDebug::PageADC::self, NamePage::DebugADCrShift,
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

DEF_PAGE_3(pageADC, PageDebug::self, NamePage::DebugADC,
    "АЦП", "ADC",
    "",
    "",
    *PageDebug::PageADC::PageBalance::self,     // ОТЛАДКА - АЦП - БАЛАНС
    *PageDebug::PageADC::PageStretch::self,     // ОТЛАДКА - АЦП - РАСТЯЖКА
    *PageDebug::PageADC::PageAltRShift::self,   // ОТЛАДКА - АЦП - ДОП СМЕЩ
    nullptr, nullptr, nullptr, nullptr
)

static void OnChanged_Randomizer_SamplesForGates()
{
    FPGA::Randomizer::SetNumberMeasuresForGates(setNRST.num_measures_for_gates);
}

DEF_GOVERNOR(mgRandomizer_SamplesForGates, PageDebug::PageRandomizer::self,
    "Выб-к/ворота", "Samples/gates",
    "",
    "",
    setNRST.num_measures_for_gates, 1, 2500, nullptr, OnChanged_Randomizer_SamplesForGates, nullptr
)

static void OnChanged_Randomizer_AltTShift0()
{
    TShift::SetDelta(setNRST.shift_T0);
}

DEF_GOVERNOR(mgRandomizer_AltTShift0, PageDebug::PageRandomizer::self,
    "tShift доп.", "tShift alt.",
    "",
    "",
    setNRST.shift_T0, 0, 510, nullptr, OnChanged_Randomizer_AltTShift0, nullptr
)

DEF_GOVERNOR(mgRandomizer_Average, PageDebug::PageRandomizer::self,
    "Усредн.", "Average",
    "",
    "",
    setNRST.num_ave_for_rand, 1, 32, nullptr, nullptr, nullptr
)

DEF_PAGE_3(pageRandomizer, PageDebug::self, NamePage::DebugRandomizer,
    "РАНД-ТОР", "RANDOMIZER",
    "",
    "",
    mgRandomizer_SamplesForGates,   // ОТЛАДКА - РАНД-ТОР - Выб-к/ворота
    mgRandomizer_AltTShift0,        // ОТЛАДКА - РАНД-ТОР - tShift доп.
    mgRandomizer_Average,           // ОТЛАДКА - РАНД-ТОР - Усредн.
    nullptr, nullptr, nullptr, nullptr
)

static void OnDraw_SizeSettings(int x, int y)
{
    Text("Размер %d", sizeof(Settings)).Draw(x + 5, y + 21, Color::BLACK);
}

DEF_CHOICE_2(mcSizeSettings, PageDebug::self,
    "Размер настроек", "Size settings",
    "Вывод размера структуры Settings",
    "Show size of struct Settings",
    "Размер", "Size",
    "Размер", "Size",
    size, nullptr, nullptr, OnDraw_SizeSettings
)

static bool IsActive_SaveFirmware()
{
    return FDrive::isConnected;
}

static void OnPress_SaveFirmware()
{
    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-53.bin", &structForWrite);

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Warnings::ShowWarningGood(Warning::FirmwareSaved);
}

DEF_BUTTON(mbSaveFirmware, PageDebug::self,
    "Сохр. прошивку", "Save firmware",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)

static void OnPress_EraseData()
{
    ROM::Data::EraseAll();
}

DEF_BUTTON(bEraseData, PageDebug::self,
    "Стереть данные", "Erase data",
    "Стирает сектора с данными",
    "Erases data sectors",
    nullptr, OnPress_EraseData
)

DEF_PAGE_7(pageDebug, PageMain::self, NamePage::Debug,
    "ОТЛАДКА", "DEBUG",
    "",
    "",

// Это то, что осталось от "старого" S8-53
    mcStats,                            // ОТЛАДКА - Статистика
    *PageDebug::PageConsole::self,      // ОТЛАДКА - КОНСОЛЬ
    *PageDebug::PageADC::self,          // ОТЛАДКА - АЦП
    *PageDebug::PageRandomizer::self,   // ОТЛАДКА - РАНД-ТОР
    mcSizeSettings,                     // ОТЛАДКА - Размер настроек
    mbSaveFirmware,                     // ОТЛАДКА - Сохр. прошивку
    bEraseData,                         // ОТЛАДКА - Стереть данные
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::self = &pageDebug;
const Page *PageDebug::PageADC::self = &pageADC;
const Page *PageDebug::PageADC::PageBalance::self = &pageBalanceADC;
const Page *PageDebug::PageADC::PageStretch::self = &pageStretchADC;
const Page *PageDebug::PageADC::PageAltRShift::self = &pageAltShiftADC;
const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
