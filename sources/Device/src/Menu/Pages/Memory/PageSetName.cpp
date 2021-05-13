// 2021/05/12 14:32:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "FDrive/FDrive.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsCommon.h"
#include <cstring>


static void PressSB_Exit()
{
    Display::RemoveAddDrawFunction();
    if (PageMemory::exitFromModeSetNameTo == RETURN_TO_LAST_MEM)
    {
        PageMemory::PageLatest::self->OpenAndSetItCurrent();
    }
    else if (PageMemory::exitFromModeSetNameTo == RETURN_TO_INT_MEM)
    {
        PageMemory::PageInternal::self->OpenAndSetItCurrent();
    }
    PageMemory::exitFromModeSetNameTo = RETURN_TO_DISABLE_MENU;
}


DEF_SMALL_BUTTON(sbExit, PageMemory::PageSetName::self,     // Кнопк для выхода из режима задания имени сохраняемому сигналу. Одновременно кнопка отказа от сохранения
    EXIT_RU, EXIT_EN,
    "Отказ от сохранения",
    "Failure to save",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void PressSB_Delete()
{
    set.memory.file_name[0] = '\0';
}


static void DrawSB_Delete(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbDelete, PageMemory::PageSetName::self,
    "Удалить", "Delete",
    "Удаляет все введённые символы",
    "Deletes all entered characters",
    nullptr, PressSB_Delete, DrawSB_Delete, nullptr
)


static void PressSB_Backspace()
{
    int size = (int)(std::strlen(set.memory.file_name));
    if (size > 0)
    {
        set.memory.file_name[size - 1] = '\0';
    }
}


static void DrawSB_Backspace(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbBackspace, PageMemory::PageSetName::self,
    "Backspace", "Backspace",
    "Удаляет последний символ",
    "Delete the last character",
    nullptr, PressSB_Backspace, DrawSB_Backspace, nullptr
)


static void PressSB_Insert()
{
    int size = (int)(std::strlen(set.memory.file_name));
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        set.memory.file_name[size] = Tables::symbolsAlphaBet[set.memory.index_cur_symbol_name_mask][0];
        set.memory.file_name[size + 1] = '\0';
    }
}


static void DrawSB_Insert(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x26').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbInsert, PageMemory::PageSetName::self,
    "Вставить", "Insert",
    "Вводит очередной символ",
    "Print the next character",
    nullptr, PressSB_Insert, DrawSB_Insert, nullptr
)


static void PressSB_Save()
{
    if (FDrive::isConnected)
    {
        PressSB_Exit();
        PageMemory::needForSaveToFlashDrive = true;
    }
}


static void DrawSB_Save(int x, int y)
{
    if (FDrive::isConnected)
    {
        Font::Set(TypeFont::UGO2);
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1);
        Font::Set(TypeFont::S8);
    }
}


DEF_SMALL_BUTTON(sbSave, PageMemory::PageSetName::self,
    "Сохранить", "Save",
    "Сохранение на флеш под заданным именем",
    "Saving to flashdrive with the specified name",
    nullptr, PressSB_Save, DrawSB_Save, nullptr
)


static void OnRegSet(int angle)
{
    PageMemory::PageSetMask::OnRegSet(angle, sizeof(Tables::symbolsAlphaBet) / 4 - 7);
}


DEF_PAGE_6(pageSetName, &Page::empty, NamePage::SB_MemExtSetName,
    "", "",
    "",
    "",
    sbExit,
    sbDelete,
    Item::empty,
    sbBackspace,
    sbInsert,
    sbSave,
    nullptr, nullptr, nullptr, OnRegSet
)

const Page *PageMemory::PageSetName::self = &pageSetName;
