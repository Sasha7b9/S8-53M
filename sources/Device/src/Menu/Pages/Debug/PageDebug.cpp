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
#include <cstdio>
#include <cstdlib>


//----------------------------------------------------------------------------------------------------------------------

DEF_CHOICE_2(cStats, PageDebug::self,
    "Статистика", "Statistics"
    ,
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в "
    "памяти/количество сохраняемых в памяти сигналов"
    ,
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity "
    "of the signals kept in memory"
    ,
    "Не показывать", "Hide",
    "Показывать", "Show",
    setNRST.show_stats, nullptr, SettingsNRST::CommonOnChanged, nullptr
)

//----------------------------------------------------------------------------------------------------------------------

static int8 size = 0;

static void OnDraw_SizeSettings(int x, int y)
{
    Text("Размер %d", sizeof(SettingsMain)).Draw(x + 5, y + 21, Color::BLACK);
}


DEF_CHOICE_2(cSizeSettings, PageDebug::self,
    "Размер настроек", "Size settings",
    "Вывод размера структуры Settings",
    "Show size of struct Settings",
    "Размер", "Size",
    "Размер", "Size",
    size, nullptr, SettingsNRST::CommonOnChanged, OnDraw_SizeSettings
)

//----------------------------------------------------------------------------------------------------------------------

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


DEF_BUTTON(bSaveFirmware, PageDebug::self,
    "Сохр. прошивку", "Save firmware",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_7(pageDebug, PageMain::self, NamePage::Debug,
    "ОТЛАДКА", "DEBUG",
    "",
    "",
    *PageDebug::PageADC::self,
    *PageDebug::PageRandomizer::self,
    *PageDebug::PageConsole::self,
    cStats,
    cSizeSettings,
    bSaveFirmware,
    bEraseData,
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageDebug::self = &pageDebug;
