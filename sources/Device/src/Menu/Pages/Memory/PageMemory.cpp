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

static void DrawSetName();  // Эта функция рисует, когда нужно задать имя файла для сохранения


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


//static void DrawSB_MemLastSelect(int x, int y)
//{
//    Font::Set(TypeFont::UGO2);
//    Char(set.memory.str_memory_last.isActiveModeSelect ? '\x2a' : '\x28').Draw4SymbolsInRect(x + 3, y + 2);
//    Font::Set(TypeFont::S8);
//}


//static void PressSB_MemLastSelect()
//{
//    set.memory.str_memory_last.isActiveModeSelect = !set.memory.str_memory_last.isActiveModeSelect;
//}


//static void DrawSB_MemExtNewFolder(int x, int y)
//{
//    Font::Set(TypeFont::UGO2);
//    Char('\x46').Draw4SymbolsInRect(x + 1, y);
//    Font::Set(TypeFont::S8);
//}


static bool FuncActiveMemoryNumPoinst()
{
    return !PeackDetMode::IsEnabled();
}

// ПАМЯТЬ - Точки
int8 temp = 0;

DEF_CHOICE_7(mcMemoryNumPoints, PageMemory::self,
    "Точки", "Points"
    ,
    "Выбор количества отсчётов для сохраняемых сигналов. "
    "При увеличении количества отсчётов уменьшается количество сохранённых в памяти сигналов."
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


static void DrawSetName()
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

    // Рисуем большие буквы английского алфавита
    while (Tables::symbolsAlphaBet[index][0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // Теперь рисуем цифры и пробел
    position = 0;
    while (Tables::symbolsAlphaBet[index][0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // Теперь рисуем малые буквы алфавита
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


void PageMemory::PageExternal::OnPress_FileManager()
{
    if(FDrive::isConnected)
    {
        PageMemory::PageFileManager::self->OpenAndSetItCurrent();
        Display::SetDrawMode(DrawMode::Hand, FM::Draw);
        FM::needRedrawFileManager = 1;
    }
}


DEF_PAGE_4(pageMemory, PageMain::self, NamePage::Memory,
    "ПАМЯТЬ", "MEMORY",
    "Работа с внешней и внутренней памятью.",
    "Working with external and internal memory.",
    mcMemoryNumPoints,
    *PageMemory::PageLatest::self,
    *PageMemory::PageInternal::self,
    *PageMemory::PageExternal::self ,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageMemory::self = &pageMemory;
