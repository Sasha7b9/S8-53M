// 2021/06/01 10:09:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


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

DEF_PAGE_2(pageData, PageDebug::self, NamePage::DebugInfo,
    "ДАННЫЕ", "DATA",
    "",
    "",
    bSaveFirmware,
    bEraseData,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageData::self = &pageData;
