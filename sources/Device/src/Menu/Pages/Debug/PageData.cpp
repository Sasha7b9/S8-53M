// 2021/06/01 10:09:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


//----------------------------------------------------------------------------------------------------------------------

static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    FDrive::OpenNewFileForWrite("S8-53M.bin");

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock);
        address += sizeBlock;
    }

    FDrive::CloseFile();

    Warnings::ShowWarningGood("ПРОШИВКА СОХРАНЕНА", "FIRMWARE SAVED");
}

DEF_BUTTON(bSaveFirmware, PageDebug::PageData::self,
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

DEF_BUTTON(bEraseData, PageDebug::PageData::self,
    "Стереть данные", "Erase data",
    "Стирает сектора с данными",
    "Erases data sectors",
    nullptr, OnPress_EraseData
)

//----------------------------------------------------------------------------------------------------------------------

static void OnPress_EraseSettings()
{
    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase();

    set.Load();
}

DEF_BUTTON(bEraseSettings, PageDebug::PageData::self,
    "Стереть настройки", "Erase settings",
    "Стереть настройки",
    "Erase settings",
    nullptr, OnPress_EraseSettings
)

//----------------------------------------------------------------------------------------------------------------------

static void OnPress_EraseNRST()
{
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();

    setNRST.Load();
}

DEF_BUTTON(bEraseNRST, PageDebug::PageData::self,
    "Стереть NRST", "Erase NRST",
    "Стереть несбрасываемые настройки",
    "Erase non-resettable settings",
    nullptr, OnPress_EraseNRST
)

//----------------------------------------------------------------------------------------------------------------------

DEF_PAGE_4(pageData, PageDebug::self, NamePage::DebugInfo,
    "ДАННЫЕ", "DATA",
    "",
    "",
    bSaveFirmware,
    bEraseData,
    bEraseSettings,
    bEraseNRST,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageData::self = &pageData;
