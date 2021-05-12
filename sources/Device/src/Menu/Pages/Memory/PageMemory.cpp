// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "common/Utils/Math_.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstring>


int16 PageMemory::PageLatest::currentSignal = 0;
int8 PageMemory::PageInternal::currentSignal = 0;
bool PageMemory::PageInternal::showAlways = false;
uint PageMemory::exitFromModeSetNameTo = 0;
bool PageMemory::needForSaveToFlashDrive = false;

bool PageMemory::exitFromIntToLast = false;

static void DrawSetName();  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������


void PageMemory::OnChanged_MemoryNumPoints(bool)
{
    uint points_in_channel = set.memory.enum_points_fpga.PointsInChannel();

    int16 &shift = set.display.shift_in_memory_in_points;

    if(points_in_channel == 281)
    {
        shift = 0;
    }
    else
    {
        if(TPos::IsLeft())
        {
            shift = 0;
        }
        else if(TPos::IsCenter())
        {
            shift = (int16)(points_in_channel / 2 - Grid::Width() / 2);
        }
        else if(TPos::IsRight())
        {
            shift = (int16)(points_in_channel - Grid::Width() - 2);
        }
    }

    TShift::Set(set.time.shift);
}

// ������� �� ������ - ����� �� - �����



void DrawSB_MemLastSelect(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(set.memory.str_memory_last.isActiveModeSelect ? '\x2a' : '\x28').Draw4SymbolsInRect(x + 3, y + 2);
    Font::Set(TypeFont::S8);
}





void PressSB_MemLastSelect()
{
    set.memory.str_memory_last.isActiveModeSelect = !set.memory.str_memory_last.isActiveModeSelect;
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


static void PressSB_MemExtSetNameSave()
{
    if (FDrive::isConnected)
    {
        PressSB_SetName_Exit();
        PageMemory::needForSaveToFlashDrive = true;
    }
}













DEF_SMALL_BUTTON(sbSetNameSave, PageMemory::PageSetName::self,
    "���������", "Save",
    "���������� �� ���� ��� �������� ������",
    "Saving to flashdrive with the specified name",
    nullptr, PressSB_MemExtSetNameSave, DrawSB_MemExtSetNameSave, nullptr
)



void DrawSB_SetName_Backspace(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}



void PressSB_SetName_Backspace()
{
    int size = (int)(std::strlen(set.memory.file_name));
    if (size > 0)
    {
        set.memory.file_name[size - 1] = '\0';
    }
}





void DrawSB_SetName_Delete(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void PressSB_SetName_Delete()
{
    set.memory.file_name[0] = '\0';
}





void DrawSB_SetName_Insert(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x26').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void PressSB_SetName_Insert()
{
    int size = (int)(std::strlen(set.memory.file_name));
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        set.memory.file_name[size] = Tables::symbolsAlphaBet[set.memory.index_cur_symbol_name_mask][0];
        set.memory.file_name[size + 1] = '\0';
    }
}











DEF_SMALL_BUTTON(sbSetNameBackspace, PageMemory::PageSetName::self,
    "Backspace", "Backspace",
    "������� ��������� ������",
    "Delete the last character",
    nullptr, PressSB_SetName_Backspace, DrawSB_SetName_Backspace, nullptr
)

DEF_SMALL_BUTTON(sbSetNameDelete, PageMemory::PageSetName::self,
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered characters",
    nullptr, PressSB_SetName_Delete, DrawSB_SetName_Delete, nullptr
)

DEF_SMALL_BUTTON(sbSetNameInsert, PageMemory::PageSetName::self,
    "��������", "Insert",
    "������ ��������� ������",
    "Print the next character",
    nullptr, PressSB_SetName_Insert, DrawSB_SetName_Insert, nullptr
)

void DrawSB_MemExtNewFolder(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x46').Draw4SymbolsInRect(x + 1, y);
    Font::Set(TypeFont::S8);
}

void DrawSB_FM_LevelDown(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void DrawSB_FM_LevelUp(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

static void DrawSB_FM_Tab(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x6e').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

DEF_SMALL_BUTTON(sbFileManagerTab, PageMemory::PageFileManager::self,
    "Tab", "Tab",
    "������� ����� ���������� � �������",
    "The transition between the directories and files",
    nullptr, FM::PressTab, DrawSB_FM_Tab, nullptr
)

DEF_SMALL_BUTTON(sbFileManagerLevelDown, PageMemory::PageFileManager::self,
    "�����", "Enter",
    "������� � ��������� �������",
    "Transition to the chosen catalog",
    nullptr, FM::PressLevelDown, DrawSB_FM_LevelDown, nullptr
)

DEF_SMALL_BUTTON(sbFileManagerLevelUp, PageMemory::PageFileManager::self,
    "�����", "Leave",
    "������� � ������������ �������",
    "Transition to the parental catalog",
    nullptr, FM::PressLevelUp, DrawSB_FM_LevelUp, nullptr
)


























































DEF_SMALL_BUTTON(sbExitSetName, PageMemory::PageSetName::self,     // ����� ��� ������ �� ������ ������� ����� ������������ �������. ������������ ������ ������ �� ����������
    EXIT_RU, EXIT_EN,
    "����� �� ����������",
    "Failure to save",
    nullptr, PressSB_SetName_Exit, DrawSB_Exit, nullptr
)




// ������� ������ - ����� �� - �����


static bool FuncActiveMemoryNumPoinst()
{
    return !PeackDetMode::IsEnabled();
}

// ������ - �����
int8 temp = 0;

DEF_CHOICE_7(mcMemoryNumPoints, PageMemory::self,
    "�����", "Points"
    ,
    "����� ���������� �������� ��� ����������� ��������. "
    "��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������."
    ,
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases."
    ,
    "281", "281",
    "512", "512",
    "1k",  "1k",
    "2k",  "2k",
    "4k",  "4k",
    "8k",  "8k",
    "16k", "16k",
    set.memory.enum_points_fpga, FuncActiveMemoryNumPoinst, PageMemory::OnChanged_MemoryNumPoints, nullptr
)



    
// ������ - ����� �� - ���������������



// ������ - ����� �� - ��� �� ������



void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (Tables::symbolsAlphaBet[index][0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (Tables::symbolsAlphaBet[index][0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (Tables::symbolsAlphaBet[index][0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = Text(set.memory.file_name).Draw(x0 + deltaX, y0 + 65, Color::FILL);
    Region(5, 8).Fill(x, y0 + 65, Color::FLASH_10);
}





void PageMemory::PageExternal::SaveSignalToFlashDrive()
{
    if (FDrive::isConnected)
    {
        if (set.memory.file_naming_mode.IsHand())
        {
            PageMemory::PageSetName::self->OpenAndSetItCurrent();
            Display::SetAddDrawFunction(DrawSetName);
        }
        else
        {
            PageMemory::needForSaveToFlashDrive = true;
        }
    }
    else
    {
        PageMemory::exitFromModeSetNameTo = RETURN_TO_MAIN_MENU;
    }
}








void PageMemory::PageExternal::OnPress_MemoryFileManager()
{
    if(FDrive::isConnected)
    {
        PageMemory::PageFileManager::self->OpenAndSetItCurrent();
        Display::SetDrawMode(DrawMode::Hand, FM::Draw);
        FM::needRedrawFileManager = 1;
    }
}

bool FuncOfActiveExtMemFolder()
{
    return FDrive::isConnected;
}

static void PressSB_FM_Exit()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbExitFileManager, PageMemory::PageFileManager::self,
    EXIT_RU, EXIT_EN,
    EXIT_ILLUSTRATION_RU,
    EXIT_ILLUSTRATION_EN,
    nullptr, PressSB_FM_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_6(pageFileManager, PageMemory::PageExternal::self, NamePage::SB_FileManager,
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    sbExitFileManager,
    sbFileManagerTab,
    Item::empty,
    Item::empty,
    sbFileManagerLevelUp,
    sbFileManagerLevelDown,
    FuncOfActiveExtMemFolder, PageMemory::PageExternal::OnPress_MemoryFileManager, EmptyFuncVV, FM::RotateRegSet
);







// �������� ���������� ��� ��������� ������ ������ ������� ����� ����� ����� ����������� �� ������ ///////////////
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

DEF_PAGE_4(pageMemory, PageMain::self, NamePage::Memory,
    "������", "MEMORY",
    "������ � ������� � ���������� �������.",
    "Working with external and internal memory.",
    mcMemoryNumPoints,
    *PageMemory::PageLatest::self,
    *PageMemory::PageInternal::self,
    *PageMemory::PageExternal::self ,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageMemory::self = &pageMemory;
const Page *PageMemory::PageSetName::self = &pageSetName;
const Page *PageMemory::PageFileManager::self = &pageFileManager;
