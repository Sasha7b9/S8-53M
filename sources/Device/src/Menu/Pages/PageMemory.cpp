#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Utils/Math_c.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include "Hardware/EPROM.h"
#include "Hardware/Sound.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/PageMemory.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include <cstring>


using namespace Primitives;


extern const Page pMemory;

extern const Page mainPage;

static void DrawSetMask();  // ��� ������� ������, ����� ������ ����� ������� �����.
static void DrawSetName();  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������


void ChangeC_Memory_NumPoints(bool)
{
    if(sMemory_GetNumPoints(false) == 281)
    {
        SHIFT_IN_MEMORY = 0;
    }
    else
    {
        if(SET_TPOS_IS_LEFT)
        {
            SHIFT_IN_MEMORY = 0;
        }
        else if(SET_TPOS_IS_CENTER)
        {
            SHIFT_IN_MEMORY = static_cast<int16>(sMemory_GetNumPoints(false) / 2 - Grid::Width() / 2);
        }
        else if(SET_TPOS_IS_RIGHT)
        {
            SHIFT_IN_MEMORY = static_cast<int16>(sMemory_GetNumPoints(false) - Grid::Width() - 2);
        }
    }
    FPGA::SetTShift(TSHIFT);
}

// ������� �� ������ - ����� �� - �����
bool IsActiveMemoryExtSetMask()
{                       
    return FILE_NAMING_MODE_IS_MASK;
}


void DrawSB_MemLastSelect(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 3, y + 2, set.memory.strMemoryLast.isActiveModeSelect ? '\x2a' : '\x28');
    Font::Set(TypeFont::_8);
}

void DrawSB_MemLast_Prev(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x20');
    Font::Set(TypeFont::_8);
}

void DrawSB_MemLast_Next(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x64');
    Font::Set(TypeFont::_8);
}

void PressSB_MemLastSelect()
{
    set.memory.strMemoryLast.isActiveModeSelect = !set.memory.strMemoryLast.isActiveModeSelect;
}

void PressSB_MemLast_Next()
{
    CircleIncreaseInt16(&CURRENT_NUM_LATEST_SIGNAL, 0, static_cast<int16>(Storage::AllDatas() - 1));
}

void PressSB_MemLast_Prev()
{
    CircleDecreaseInt16(&CURRENT_NUM_LATEST_SIGNAL, 0, static_cast<int16>(Storage::AllDatas() - 1));
}

static void RotateSB_MemLast(int angle)
{
    if (Storage::AllDatas() > 1)
    {
        Sound::RegulatorSwitchRotate();
    }
    if (Math::Sign(angle) > 0)
    {
        PressSB_MemLast_Next();
    }
    else
    {
        PressSB_MemLast_Prev();
    }
}

static void FuncDrawingAdditionSPageMemoryLast()
{
    char buffer[20];
    
    int width = 40;
    int height = 10;
    Region(width, height).Fill(Grid::Right() - width, Grid::TOP, COLOR_BACK);
    Rectangle(width, height).Draw(Grid::Right() - width, Grid::TOP, COLOR_FILL);
    Text(Int2String(CURRENT_NUM_LATEST_SIGNAL + 1, false, 3, buffer)).Draw(Grid::Right() - width + 2, Grid::TOP + 1);
    Text("/").Draw(Grid::Right() - width + 17, Grid::TOP + 1);
    Text(Int2String(Storage::AllDatas(), false, 3, buffer)).Draw(Grid::Right() - width + 23, Grid::TOP + 1);
}

void DrawSB_MemLast_IntEnter(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x40');
    Font::Set(TypeFont::_8);
}

void DrawSB_MemLast_SaveToFlash(int x, int y)
{
    if (FLASH_DRIVE_IS_CONNECTED)
    {
        Font::Set(TypeFont::_UGO2);
        Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x42');
        Font::Set(TypeFont::_8);
    }
}

static void DrawSB_MemExtSetNameSave(int x, int y)
{
    if (FLASH_DRIVE_IS_CONNECTED)
    {
        Font::Set(TypeFont::_UGO2);
        Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x42');
        Font::Set(TypeFont::_8);
    }
}

//const PageSB pageSBmemExtSetName;

static void PressSB_MemLast_SaveToFlash()
{
    EXIT_FROM_SET_NAME_TO = RETURN_TO_LAST_MEM;
    Memory_SaveSignalToFlashDrive();
}


static void PressSB_SetName_Exit()
{
    Display::RemoveAddDrawFunction();
    if (EXIT_FROM_SET_NAME_TO_DIS_MENU)
    {
        Menu::ShortPressOnPageItem(Menu::PagePointerFromName(NamePage::SB_MemExtSetName), 0);
    }
    else if (EXIT_FROM_SET_NAME_TO_LAST)
    {
        Menu::OpenPageAndSetItCurrent(PageMemory::Latest::GetPointer());
    }
    else if (EXIT_FROM_SET_NAME_TO_INT)
    {
        Menu::OpenPageAndSetItCurrent(PageMemory::Internal::GetPointer());
    }
    EXIT_FROM_SET_NAME_TO = RETURN_TO_DISABLE_MENU;
}


static void PressSB_MemExtSetNameSave()
{
    if (FLASH_DRIVE_IS_CONNECTED)
    {
        PressSB_SetName_Exit();
        NEED_SAVE_TO_DRIVE = 1;
    }
}

extern const Page mspMemLast;

DEF_SMALL_BUTTON(sbMemLastPrev, mspMemLast,
    "����������", "Previous",
    "������� � ����������� �������",
    "Go to the previous signal",
    nullptr, PressSB_MemLast_Prev, DrawSB_MemLast_Prev, nullptr
)


DEF_SMALL_BUTTON(sbMemLastNext, mspMemLast,
    "���������", "Next",
    "������� � ���������� �������",
    "Go to the next signal",
    nullptr, PressSB_MemLast_Next, DrawSB_MemLast_Next, nullptr
)


void PressSB_MemLast_IntEnter()
{
    Menu::OpenPageAndSetItCurrent(PageMemory::Internal::GetPointer());
    MODE_WORK = ModeWork::MemInt;
    EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSmemInt, &gData0memInt, &gData1memInt);
    EXIT_FROM_INT_TO_LAST = 1;
}

DEF_SMALL_BUTTON(sbMemLastIntEnter, mspMemLast,
    "����� ��", "Internal storage",
    "������� ��� ������, ����� ��������� ������ �� ���������� ������������ ����������",
    "Press this button to keep a signal in an internal memory",
    nullptr, PressSB_MemLast_IntEnter, DrawSB_MemLast_IntEnter, nullptr
)

DEF_SMALL_BUTTON(sbMemLastSaveToFlash, mspMemLast,
    "���������", "Save",
    "������ ���������� �������� ��� ������������� ������� ��. ��������� ��������� ������ �� ������ ��",
    "Click this button to save the signal on the external FLASH",
    nullptr, PressSB_MemLast_SaveToFlash, DrawSB_MemLast_SaveToFlash, nullptr
)


extern const Page mpSetName;

DEF_SMALL_BUTTON(sbSetNameSave, mpSetName,
    "���������", "Save",
    "���������� �� ���� ��� �������� ������",
    "Saving to flashdrive with the specified name",
    nullptr, PressSB_MemExtSetNameSave, DrawSB_MemExtSetNameSave, nullptr
)


void DrawSB_SetMask_Backspace(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    Font::Set(TypeFont::_8);
}

void DrawSB_SetName_Backspace(int x, int y) //-V524
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    Font::Set(TypeFont::_8);
}

void PressSB_SetMask_Backspace()
{
    int size = static_cast<int>(std::strlen(FILE_NAME_MASK));
    if (size > 0)
    {
        if (size > 1 && FILE_NAME_MASK[size - 2] == 0x07)
        {
            FILE_NAME_MASK[size - 2] = '\0';
        }
        else
        {
            FILE_NAME_MASK[size - 1] = '\0';
        }
    }

}

void PressSB_SetName_Backspace()
{
    int size = static_cast<int>(std::strlen(FILE_NAME));
    if (size > 0)
    {
        FILE_NAME[size - 1] = '\0';
    }
}

void DrawSB_SetMask_Delete(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Font::Set(TypeFont::_8);
}

void PressSB_SetMask_Delete()
{
    FILE_NAME_MASK[0] = '\0';
}

void DrawSB_SetName_Delete(int x, int y) //-V524
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Font::Set(TypeFont::_8);
}

void PressSB_SetName_Delete()
{
    FILE_NAME[0] = '\0';
}

void DrawSB_SetMask_Insert(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_INSERT);
    Font::Set(TypeFont::_8);
}

void PressSB_SetMask_Insert()
{
    int index = INDEX_SYMBOL;
    int size = static_cast<int>(std::strlen(FILE_NAME_MASK));
    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }
    if (index < 0x41)
    {
        FILE_NAME_MASK[size] = symbolsAlphaBet[index][0];
        FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // ��� %nN - ��������� ������
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (FILE_NAME_MASK[size - 1] >= 0x30 && FILE_NAME_MASK[size - 1] <= 0x39) // ���� ����� ������� �����
                {
                    FILE_NAME_MASK[size] = FILE_NAME_MASK[size - 1] - 0x30;
                    FILE_NAME_MASK[size - 1] = 0x07;
                    FILE_NAME_MASK[size + 1] = '\0'; 
                }
            }
        }
        else
        {
            FILE_NAME_MASK[size] = (int8)index;
            FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}

void DrawSB_SetName_Insert(int x, int y) //-V524
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x26');
    Font::Set(TypeFont::_8);
}

void PressSB_SetName_Insert()
{
    int size = static_cast<int>(std::strlen(FILE_NAME));
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        FILE_NAME[size] = symbolsAlphaBet[INDEX_SYMBOL][0];
        FILE_NAME[size + 1] = '\0';
    }
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    int8(*func[3])(int8 *, int8, int8) =
    {
        CircleDecreaseInt8,
        CircleDecreaseInt8,
        CircleIncreaseInt8
    };

    Painter::ResetFlash();
    if (INDEX_SYMBOL > maxIndex)
    {
        INDEX_SYMBOL = static_cast<int8>(maxIndex - 1);
    }
    func[Math::Sign(angle) + 1](&INDEX_SYMBOL, 0, static_cast<int8>(maxIndex - 1));
    Sound::RegulatorSwitchRotate();

}

static void OnMemExtSetMaskRegSet(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4);
}

extern const Page mspSetMask;


DEF_SMALL_BUTTON(sbSetMaskBackspace, mspSetMask,
    "Backspace", "Backspace",
    "������� ��������� �������� ������",
    "Deletes the last entered symbol",
    nullptr, PressSB_SetMask_Backspace, DrawSB_SetMask_Backspace, nullptr
)

DEF_SMALL_BUTTON(sbSetMaskDelete, mspSetMask,
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered symbols",
    nullptr, PressSB_SetMask_Delete, DrawSB_SetMask_Delete, nullptr
)

DEF_SMALL_BUTTON(sbSetMaskInsert, mspSetMask,
    "��������", "Insert",
    "��������� ��������� ������",
    "Inserts the chosen symbol",
    nullptr, PressSB_SetMask_Insert, DrawSB_SetMask_Insert, nullptr
)


DEF_SMALL_BUTTON(sbSetNameBackspace, mpSetName,
    "Backspace", "Backspace",
    "������� ��������� ������",
    "Delete the last character",
    nullptr, PressSB_SetName_Backspace, DrawSB_SetName_Backspace, nullptr
)

DEF_SMALL_BUTTON(sbSetNameDelete, mpSetName,
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered characters",
    nullptr, PressSB_SetName_Delete, DrawSB_SetName_Delete, nullptr
)

DEF_SMALL_BUTTON(sbSetNameInsert, mpSetName,
    "��������", "Insert",
    "������ ��������� ������",
    "Print the next character",
    nullptr, PressSB_SetName_Insert, DrawSB_SetName_Insert, nullptr
)

void DrawSB_MemExtNewFolder(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 1, y, '\x46');
    Font::Set(TypeFont::_8);
}

void DrawSB_FM_LevelDown(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    Font::Set(TypeFont::_8);
}

void DrawSB_FM_LevelUp(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    Font::Set(TypeFont::_8);
}

static void DrawSB_FM_Tab(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x6e');
    Font::Set(TypeFont::_8);
}

extern const Page mspFileManager;

DEF_SMALL_BUTTON(sbFileManagerTab, mspFileManager,
    "Tab", "Tab",
    "������� ����� ���������� � �������",
    "The transition between the directories and files",
    nullptr, FM::PressTab, DrawSB_FM_Tab, nullptr
)

DEF_SMALL_BUTTON(sbFileManagerLevelDown, mspFileManager,
    "�����", "Enter",
    "������� � ��������� �������",
    "Transition to the chosen catalog",
    nullptr, FM::PressLevelDown, DrawSB_FM_LevelDown, nullptr
)

DEF_SMALL_BUTTON(sbFileManagerLevelUp, mspFileManager,
    "�����", "Leave",
    "������� � ������������ �������",
    "Transition to the parental catalog",
    nullptr, FM::PressLevelUp, DrawSB_FM_LevelUp, nullptr
)

void DrawSB_MemInt_SaveToIntMemory(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Font::Set(TypeFont::_8);
}


static void SaveSignalToIntMemory(void)
{
    if (EXIT_FROM_INT_TO_LAST)          // ���� ������� �� ����� �� �� ���������
    {
        if  (gDSmemLast != 0)
        {                               // �� ��������� ������ �� ���������
            EPROM::SaveData(CURRENT_NUM_INT_SIGNAL, gDSmemLast, gData0memLast, gData1memLast);
            EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSmemInt, &gData0memInt, &gData1memInt);
            Display::ShowWarningGood(Warning::SignalIsSaved);
        }
    }
    else                                // ����� ��������� ������� ������
    {
        if (gDSet != 0)
        {
            EPROM::SaveData(CURRENT_NUM_INT_SIGNAL, gDSet, gData0, gData1);
            EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSet, &gData0memInt, &gData1memInt);
            Display::ShowWarningGood(Warning::SignalIsSaved);
        }
    }
}


void PressSB_MemInt_SaveToIntMemory()
{
    SaveSignalToIntMemory();
}

void DrawSB_MemInt_SaveToFlashDrive(int x, int y) //-V524
{
    if (FLASH_DRIVE_IS_CONNECTED)
    {
        Font::Set(TypeFont::_UGO2);
        Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x42');
        Font::Set(TypeFont::_8);
    }
}

static void DrawMemoryWave(int num, bool exist)
{
    char buffer[20];
    
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 10;
    int width = 12;
    Region(width, 10).Fill(x, y, num == CURRENT_NUM_INT_SIGNAL ? Color::FLASH_10 : COLOR_BACK);
    Rectangle(width, 10).Draw(x, y, COLOR_FILL);
    Color::SetCurrent(num == CURRENT_NUM_INT_SIGNAL ? Color::FLASH_01 : COLOR_FILL);
    if (exist)
    {
        Text(Int2String(num + 1, false, 2, buffer)).Draw(x + 2, y + 1);
    }
    else
    {
        Text("\x88").Draw(x + 3, y + 1);
    }
}

static void FuncAdditionDrawingSPageMemoryInt()
{
    // ������ �������� ��������� ������

    bool exist[MAX_NUM_SAVED_WAVES] = {false};

    EPROM::GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave(i, exist[i]);
    }
}

void PressSB_MemInt_SaveToFlashDrive()
{
    EXIT_FROM_SET_NAME_TO = RETURN_TO_INT_MEM;
    Memory_SaveSignalToFlashDrive();
}

static void FuncOnRegSetMemInt(int delta)
{
    Sound::RegulatorSwitchRotate();
    if (delta < 0)
    {
        CircleDecreaseInt8(&CURRENT_NUM_INT_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        CircleIncreaseInt8(&CURRENT_NUM_INT_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSmemInt, &gData0memInt, &gData1memInt);
    Painter::ResetFlash();
}

static void DrawSB_MemInt_ShowSignalAllways_Yes(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x66');
    Font::Set(TypeFont::_8);
}

static void DrawSB_MemInt_ShowSignalAllways_No(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x68');
    Font::Set(TypeFont::_8);
}

static void DrawSB_MemInt_ShowSignalAlways(int x, int y)
{
    if (INT_SHOW_ALWAYS == 0)
    {
        DrawSB_MemInt_ShowSignalAllways_No(x, y);
    }
    else
    {
        DrawSB_MemInt_ShowSignalAllways_Yes(x, y);
    }
}

static void PressSB_MemInt_ShowSignalAlways()
{
    INT_SHOW_ALWAYS = (INT_SHOW_ALWAYS == 0) ? 1U : 0U;
}

extern const Page mspMemInt;

static const arrayHints hintsMemIntShowSignalAlways =
{
    {DrawSB_MemInt_ShowSignalAllways_Yes, "���������� ��������� ������ �� ���������� ������ ������ ��������",
                                          "to show the chosen signal from internal memory over the current"},
    {DrawSB_MemInt_ShowSignalAllways_No,  "������ �� ���������� ������ ����� ������ � ������ ������ � ���������� ������������ �����������",
                                          "the signal from internal memory is visible only in an operating mode with an internal memory"}
};

DEF_SMALL_BUTTON(sbMemIntShowSignalAlways, mspMemInt,
    "���������� ������", "To show always",
    "��������� ������ ���������� ��������� ���������� ������ ������ ��������",
    "Allows to show always the chosen kept signal over the current",
    nullptr, PressSB_MemInt_ShowSignalAlways, DrawSB_MemInt_ShowSignalAlways, &hintsMemIntShowSignalAlways
)


static void DrawSB_MemInt_ModeShow_Direct(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_MEM_INT_SHOW_DIRECT);
    Font::Set(TypeFont::_8);
}

static void DrawSB_MemInt_ModeShow_Saved(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_MEM_INT_SHOW_SAVED);
    Font::Set(TypeFont::_8);
}

static void DrawSB_MemInt_ModeShow_Both(int x, int y)
{
    ++y;

    Rectangle(4, 7).Draw(x + 2, y + 5);

    Rectangle(4, 4).Draw(x + 8, y + 8);
    HLine().Draw(x + 8, y + 8, y + 5);
    HLine().Draw(y + 5, x + 8, x + 12);

    HLine().Draw(x + 14, y + 6, y + 12);
    HLine().Draw(x + 17, y + 5, y + 12);
    HLine().Draw(y + 9, x + 14, x + 17);
    HLine().Draw(y + 5, x + 15, x + 17);
}

static void DrawSB_MemInt_ModeShow(int x, int y)
{
    ModeShowIntMem::E mode = MODE_SHOW_MEMINT;

    if (mode == ModeShowIntMem::Direct)
    {
        DrawSB_MemInt_ModeShow_Direct(x, y);
    }
    else if (mode == ModeShowIntMem::Saved)
    {
        DrawSB_MemInt_ModeShow_Saved(x, y);
    }
    else if (mode == ModeShowIntMem::Both)
    {
        DrawSB_MemInt_ModeShow_Both(x, y);
    }
}

static void PressSB_MemInt_ModeShow()
{
    CircleIncreaseInt8((int8*)&MODE_SHOW_MEMINT, 0, 2);
}

static const arrayHints hintsMemIntModeShow =
{
    { DrawSB_MemInt_ModeShow_Direct, "�� ������� ������� ������",     "on the display current signal" },
    { DrawSB_MemInt_ModeShow_Saved,  "�� ������� ���������� ������", "on the display the kept signal" },
    { DrawSB_MemInt_ModeShow_Both,   "�� ������� ��� �������",        "on the display the both signals" }
};

DEF_SMALL_BUTTON(sbMemIntModeShow, mspMemInt,
    "��� �������", "Type of a signal",
    "���������� ���������� ��� ������� ������ � ������ ����� ��",
    "Show recorded or current signal in mode Internal Memory",
    nullptr, PressSB_MemInt_ModeShow, DrawSB_MemInt_ModeShow, &hintsMemIntModeShow
)


static void PressSB_MemInt_Delete()
{
    EPROM::DeleteData(CURRENT_NUM_INT_SIGNAL);
}

static void DrawSB_MemInt_Delete(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    Font::Set(TypeFont::_8);
}

DEF_SMALL_BUTTON(sbMemIntDelete, mspMemInt,
    "������� ������", "Delete signal",
    "������� ������",
    "Delete signal",
    nullptr, PressSB_MemInt_Delete, DrawSB_MemInt_Delete, nullptr
)


DEF_SMALL_BUTTON(sbMemIntSave, mspMemInt,
    "���������", "Save",
    "��������� ������ �� ���������� ������������ ����������",
    "To keep a signal in an internal memory",
    nullptr, PressSB_MemInt_SaveToIntMemory, DrawSB_MemInt_SaveToIntMemory, nullptr
)

DEF_SMALL_BUTTON(sbMemIntSaveToFlash, mspMemInt,
    "���������", "Save",
    "��������� ������ �� ������",
    "Save signal to flash drive",
    nullptr, PressSB_MemInt_SaveToFlashDrive, DrawSB_MemInt_SaveToFlashDrive, nullptr
)

void PressSB_MemInt_Exit()
{
    EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSmemInt, &gData0memInt, &gData1memInt);
    if (EXIT_FROM_INT_TO_LAST)
    {
        Menu::OpenPageAndSetItCurrent(PageMemory::Latest::GetPointer());
        MODE_WORK = ModeWork::Latest;
        EXIT_FROM_INT_TO_LAST = 0;
        NEED_CLOSE_PAGE_SB = 0;
    }
    else
    {
        MODE_WORK = ModeWork::Direct;
        Menu::ShortPressOnPageItem(Menu::PagePointerFromName(NamePage::SB_MemInt), 0);
    }
}

extern const Page mpSetName;

DEF_SMALL_BUTTON(sbExitSetName, mpSetName,     // ����� ��� ������ �� ������ ������� ����� ������������ �������. ������������ ������ ������ �� ����������
    EXIT_RU, EXIT_EN,
    "����� �� ����������",
    "Failure to save",
    nullptr, PressSB_SetName_Exit, DrawSB_Exit, nullptr
)

extern const Page mspMemInt;

DEF_SMALL_BUTTON(sbExitMemInt, mspMemInt,    // ������ ��� ������ �� ������ ����� ������.
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_MemInt_Exit, DrawSB_Exit, nullptr
)


// ������� ������ - ����� �� - �����
void OnPressMemoryExtMask(void)
{
    Menu::OpenPageAndSetItCurrent(PageMemory::SetMask::GetPointer());
    Display::SetAddDrawFunction(DrawSetMask);
}

// ������
extern const Page pMemory;

static bool FuncActiveMemoryNumPoinst(void)
{
    return PEAKDET_IS_DISABLE;
}

// ������ - �����
int8 temp = 0;

DEF_CHOICE_3(mcMemoryNumPoints, pMemory,
    "�����", "Points"
    ,
    "����� ���������� �������� ��� ����������� ��������. "
    "��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������."
    ,
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases."
    ,
    "281",  "281",
    "512",  "512",
    "1024", "1024",
    ENUM_POINTS, FuncActiveMemoryNumPoinst, ChangeC_Memory_NumPoints, nullptr
)

// ������ - ����� �� /////
extern const Page mspMemoryExt;

// ������ - ����� �� - ��� �����
DEF_CHOICE_2(mcMemoryExtName, mspMemoryExt,
    "��� �����", "File name"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "�� �����", "Mask",
    "�������",  "Manually",
    FILE_NAMING_MODE, nullptr, nullptr, nullptr
)

    
// ������ - ����� �� - ���������������
DEF_CHOICE_2(mcMemoryExtAutoConnectFlash, mspMemoryExt,
    "���������.", "AutoConnect",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FLASH_AUTOCONNECT, nullptr, nullptr, nullptr
)


// ������ - ����� �� - ��� �� ������
DEF_CHOICE_2(mcMemoryExtModeBtnMemory, mspMemoryExt,
    "��� �� ������", "Mode btn MEMORY",
    "",
    "",
    "����",       "Menu",
    "����������", "Save",
    MODE_BTN_MEMORY, nullptr, nullptr, nullptr
)


// ������ - ����� �� - ��������� ���
DEF_CHOICE_2(mcMemoryExtModeSave, mspMemoryExt,
    "��������� ���", "Save as"
    ,
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � ���������� TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension BMP\n"
    "If you select \"Text\", the signal will be stored in the current directory as a text file with the extension TXT"
    ,
    "�����������", "Image",
    "�����",       "Text",
    MODE_SAVE_SIGNAL, nullptr, nullptr, nullptr
)

    
void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Rectangle(width, height).Draw(x0, y0, COLOR_FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, COLOR_BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (symbolsAlphaBet[index][0] != '%')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = Text(FILE_NAME).Draw(x0 + deltaX, y0 + 65, COLOR_FILL);
    Region(5, 8).Fill(x, y0 + 65, Color::FLASH_10);
}

static void DrawFileMask(int x, int y)
{
    char *ch = FILE_NAME_MASK;

    Color::SetCurrent(COLOR_FILL);
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            x = Char(*ch).Draw(x, y);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = Char('%').Draw(x, y);
                x = Char((char)(0x30 | *(ch + 1))).Draw(x, y);
                x = Char('N').Draw(x, y);
                ch++;
            }
            else
            {
                x = Text(symbolsAlphaBet[*ch + 0x40]).Draw(x, y);
            }
        }
        ch++;
    }
    Region(5, 8).Fill(x, y, Color::FLASH_10);
}

void DrawSetMask()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Rectangle(width, height).Draw(x0, y0, COLOR_FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, COLOR_BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while(symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }
    
    // ������ ������ ����� � ������
    position = 0;
    while(symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while(symbolsAlphaBet[index][0] != '%')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // ������ ������ �����������
    position = 0;
    while (index < (sizeof(symbolsAlphaBet) / 4))
    {
        DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static const char* strings[] =
    {
        "%y - ���, %m - �����, %d - ����",
        "%H - ����, %M - ������, %S - �������",
        "%nN - ���������� �����, ���",
        "n - ����������� ���������� ������ ��� N"
    };

    deltaY--;
    Color::SetCurrent(COLOR_FILL);
    for(int i = 0; i < sizeof(strings) / 4; i++)
    {
        Text(strings[i]).Draw(x0 + deltaX, y0 + 100 + deltaY * i);
    }
}

void Memory_SaveSignalToFlashDrive()
{
    if (FLASH_DRIVE_IS_CONNECTED)
    {
        if (FILE_NAMING_MODE_IS_HAND)
        {
            Menu::OpenPageAndSetItCurrent(PageMemory::SetName::GetPointer());
            Display::SetAddDrawFunction(DrawSetName);
        }
        else
        {
            NEED_SAVE_TO_DRIVE = 1;
        }
    }
    else
    {
        EXIT_FROM_SET_NAME_TO = RETURN_TO_MAIN_MENU;
    }
}

static void PressSB_MemLast_Exit()
{
    MODE_WORK = ModeWork::Direct;
    if (RUN_FPGA_AFTER_SMALL_BUTTONS)
    {
        FPGA::Start();
        RUN_FPGA_AFTER_SMALL_BUTTONS = 0;
    }
    Display::RemoveAddDrawFunction();
}

// ������� ������ - ���������.
void OnPressMemoryLatest()
{
    CURRENT_NUM_LATEST_SIGNAL = 0;
    RUN_FPGA_AFTER_SMALL_BUTTONS = FPGA::IsRunning() ? 1U : 0U;
    FPGA::Stop(false);
    MODE_WORK = ModeWork::Latest;
}

DEF_SMALL_BUTTON(sbExitMemLast, mspMemLast,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_MemLast_Exit, DrawSB_Exit, nullptr
)

// ������ - ��������� //////
static const arrayItems itemsMemLast =
{
};

DEF_PAGE_6(mspMemLast, pMemory, NamePage::SB_MemLatest,
    "���������", "LATEST",
    "������� � ����� ������ � ���������� ����������� ���������",
    "Transition to an operating mode with the last received signals",
    sbExitMemLast,
    Item::empty,
    sbMemLastNext,
    sbMemLastPrev,
    sbMemLastIntEnter,
    sbMemLastSaveToFlash,
    nullptr, OnPressMemoryLatest, FuncDrawingAdditionSPageMemoryLast, RotateSB_MemLast
)

static void PressSB_SetMask_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbExitSetMask, mspSetMask,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_SetMask_Exit, DrawSB_Exit, nullptr
)

// ������ - ����� �� - ����� //////////////////////////
DEF_PAGE_6(mspSetMask, mspMemoryExt, NamePage::SB_MemExtSetMask,
    "�����", "MASK",
    "����� ����� ����� ��� ��������������� ���������� ������",
    "Input mode mask for automatic file naming",
    sbExitSetMask,
    sbSetMaskDelete,
    Item::empty,
    Item::empty,
    sbSetMaskBackspace,
    sbSetMaskInsert,
    IsActiveMemoryExtSetMask, OnPressMemoryExtMask, nullptr, OnMemExtSetMaskRegSet
)

// ������ - ����� �� - ������� /////////////////////
void OnPressMemoryExtFileManager()
{
    if(FLASH_DRIVE_IS_CONNECTED)
    {
        Menu::OpenPageAndSetItCurrent(PageMemory::FileManager::GetPointer());
        Display::SetDrawMode(DrawMode::Hand, FM::Draw);
        NEED_REDRAW_FILEMANAGER = 1;
    }
}

bool FuncOfActiveExtMemFolder()
{
    return FLASH_DRIVE_IS_CONNECTED == 1;
}

static void PressSB_FM_Exit()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbExitFileManager, mspFileManager,
    EXIT_RU, EXIT_EN,
    EXIT_ILLUSTRATION_RU,
    EXIT_ILLUSTRATION_EN,
    nullptr, PressSB_FM_Exit, DrawSB_Exit, nullptr
)

static const arrayItems itemsFileManager =
{

};

const void *pMspFileManager = (const void *)&mspFileManager;

DEF_PAGE_6(mspFileManager, mspMemoryExt, NamePage::SB_FileManager,
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    sbExitFileManager,
    sbFileManagerTab,
    Item::empty,
    Item::empty,
    sbFileManagerLevelUp,
    sbFileManagerLevelDown,
    FuncOfActiveExtMemFolder, OnPressMemoryExtFileManager, EmptyFuncVV, FM::RotateRegSet
);

// ������ - ����� �� ///////
DEF_PAGE_6(mspMemoryExt, pMemory, NamePage::MemoryExt,
    "����� ��", "EXT STORAGE",
    "������ � ������� ������������ �����������.",
    "Work with external storage device.",
    mspFileManager,
    mcMemoryExtName,
    mspSetMask,
    mcMemoryExtModeSave,
    mcMemoryExtModeBtnMemory,
    mcMemoryExtAutoConnectFlash,
    nullptr, nullptr, nullptr, nullptr
);

// ������ - ����� �� 

// ������� ������ - ����� ��
void OnPressMemoryInt()
{
    Menu::OpenPageAndSetItCurrent(PageMemory::Internal::GetPointer());
    MODE_WORK = ModeWork::MemInt;
    EPROM::GetData(CURRENT_NUM_INT_SIGNAL, &gDSmemInt, &gData0memInt, &gData1memInt);
}

DEF_PAGE_6(mspMemInt, pMemory, NamePage::SB_MemInt,
    "����� ��", "INT STORAGE",
    "������� � ����� ������ � ���������� �������",
    "Transition to an operating mode with internal memory",
    sbExitMemInt,
    sbMemIntShowSignalAlways,
    sbMemIntModeShow,
    sbMemIntDelete,
    sbMemIntSave,
    sbMemIntSaveToFlash,
    nullptr, OnPressMemoryInt, FuncAdditionDrawingSPageMemoryInt, FuncOnRegSetMemInt
);

// �������� ���������� ��� ��������� ������ ������ ������� ����� ����� ����� ����������� �� ������ ///////////////
static void OnMemExtSetNameRegSet(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4 - 7);
}

DEF_PAGE_6(mpSetName, Page::empty, NamePage::SB_MemExtSetName,
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

// ������ ///////////////////
DEF_PAGE_4(pMemory, mainPage, NamePage::Memory,
    "������", "MEMORY",
    "������ � ������� � ���������� �������.",
    "Working with external and internal memory.",
    mcMemoryNumPoints,
    mspMemLast,
    mspMemInt,
    mspMemoryExt,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageMemory::self = &pMemory;


void *PageMemory::GetPointer()
{
    return (void *)&pMemory;
}


void *PageMemory::Latest::GetPointer()
{
    return (void *)&mspMemLast;
}


void *PageMemory::Internal::GetPointer()
{
    return (void *)&mspMemInt;
}


void *PageMemory::SetMask::GetPointer()
{
    return (void *)&mspSetMask;
}


void *PageMemory::SetName::GetPointer()
{
    return (void *)&mpSetName;
}


void *PageMemory::FileManager::GetPointer()
{
    return (void *)&mspFileManager;
}
