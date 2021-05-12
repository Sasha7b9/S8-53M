// 2021/05/12 14:32:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "FDrive/FDrive.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstring>


static void PressSB_SetName_Exit()
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


DEF_SMALL_BUTTON(sbExitSetName, PageMemory::PageSetName::self,     // ����� ��� ������ �� ������ ������� ����� ������������ �������. ������������ ������ ������ �� ����������
    EXIT_RU, EXIT_EN,
    "����� �� ����������",
    "Failure to save",
    nullptr, PressSB_SetName_Exit, DrawSB_Exit, nullptr
)


void PressSB_SetName_Delete()
{
    set.memory.file_name[0] = '\0';
}


void DrawSB_SetName_Delete(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbSetNameDelete, PageMemory::PageSetName::self,
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered characters",
    nullptr, PressSB_SetName_Delete, DrawSB_SetName_Delete, nullptr
)


void PressSB_SetName_Backspace()
{
    int size = (int)(std::strlen(set.memory.file_name));
    if (size > 0)
    {
        set.memory.file_name[size - 1] = '\0';
    }
}


void DrawSB_SetName_Backspace(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbSetNameBackspace, PageMemory::PageSetName::self,
    "Backspace", "Backspace",
    "������� ��������� ������",
    "Delete the last character",
    nullptr, PressSB_SetName_Backspace, DrawSB_SetName_Backspace, nullptr
)


void PressSB_SetName_Insert()
{
    int size = (int)(std::strlen(set.memory.file_name));
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        set.memory.file_name[size] = Tables::symbolsAlphaBet[set.memory.index_cur_symbol_name_mask][0];
        set.memory.file_name[size + 1] = '\0';
    }
}


void DrawSB_SetName_Insert(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x26').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbSetNameInsert, PageMemory::PageSetName::self,
    "��������", "Insert",
    "������ ��������� ������",
    "Print the next character",
    nullptr, PressSB_SetName_Insert, DrawSB_SetName_Insert, nullptr
)


static void PressSB_MemExtSetNameSave()
{
    if (FDrive::isConnected)
    {
        PressSB_SetName_Exit();
        PageMemory::needForSaveToFlashDrive = true;
    }
}


static void DrawSB_MemExtSetNameSave(int x, int y)
{
    if (FDrive::isConnected)
    {
        Font::Set(TypeFont::UGO2);
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1);
        Font::Set(TypeFont::S8);
    }
}


DEF_SMALL_BUTTON(sbSetNameSave, PageMemory::PageSetName::self,
    "���������", "Save",
    "���������� �� ���� ��� �������� ������",
    "Saving to flashdrive with the specified name",
    nullptr, PressSB_MemExtSetNameSave, DrawSB_MemExtSetNameSave, nullptr
)


static void OnMemExtSetNameRegSet(int angle)
{
    PageMemory::OnMemExtSetMaskNameRegSet(angle, sizeof(Tables::symbolsAlphaBet) / 4 - 7);
}


DEF_PAGE_6(pageSetName, &Page::empty, NamePage::SB_MemExtSetName,
    "", "",
    "",
    "",
    sbExitSetName,
    sbSetNameDelete,
    Item::empty,
    sbSetNameBackspace,
    sbSetNameInsert,
    sbSetNameSave,
    nullptr, nullptr, nullptr, OnMemExtSetNameRegSet
)

const Page *PageMemory::PageSetName::self = &pageSetName;
