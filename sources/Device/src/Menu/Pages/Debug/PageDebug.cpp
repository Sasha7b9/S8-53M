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

const Page *PageDebug::PageRandomizer::self = &pageRandomizer;
