#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Display/Grid.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/Storage.h"
#include "Hardware/EPROM.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cstring>


using namespace Primitives;


int16 PageMemory::PageLatest::currentSignal = 0;
int8 PageMemory::PageInternal::currentSignal = 0;
bool PageMemory::PageInternal::showAlways = false;
uint PageMemory::exitFromModeSetNameTo = 0;
bool PageMemory::needForSaveToFlashDrive = false;

static bool runningFPGAbeforeSmallButtons = false;      // Здесь сохраняется информация о том, работала ли ПЛИС перед переходом в режим работы с памятью
static bool exitFromIntToLast = false;                  // Если 1, то выходить из страницы внутренней памяти нужно не стандартно, а в меню последних


static void DrawSetMask();  // Эта функция рисует, когда выбран режим задания маски.
static void DrawSetName();  // Эта функция рисует, когда нужно задать имя файла для сохранения


void ChangeC_Memory_NumPoints(bool)
{
    if(sMemory_GetNumPoints(false) == 281)
    {
        SHIFT_IN_MEMORY = 0;
    }
    else
    {
        if(TPos::IsLeft())
        {
            SHIFT_IN_MEMORY = 0;
        }
        else if(TPos::IsCenter())
        {
            SHIFT_IN_MEMORY = static_cast<int16>(sMemory_GetNumPoints(false) / 2 - Grid::Width() / 2);
        }
        else if(SET_TPOS_IS_RIGHT)
        {
            SHIFT_IN_MEMORY = static_cast<int16>(sMemory_GetNumPoints(false) - Grid::Width() - 2);
        }
    }

    TShift::Set(TShift::Get());
}

// Активна ли ПАМЯТЬ - ВНЕШН ЗУ - Маска
bool IsActiveMemoryExtSetMask()
{                       
    return FILE_NAMING_MODE_IS_MASK;
}


void DrawSB_MemLastSelect(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(set.memory.strMemoryLast.isActiveModeSelect ? '\x2a' : '\x28').Draw4SymbolsInRect(x + 3, y + 2);
    Font::Set(TypeFont::S8);
}

void DrawSB_MemLast_Prev(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x20').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void DrawSB_MemLast_Next(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x64').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void PressSB_MemLastSelect()
{
    set.memory.strMemoryLast.isActiveModeSelect = !set.memory.strMemoryLast.isActiveModeSelect;
}

void PressSB_MemLast_Next()
{
//    GF::CircleIncrease<int16>(&PageMemory::PageLatest::currentSignal, 0, static_cast<int16>(Storage::AllDatas() - 1));
}

void PressSB_MemLast_Prev()
{
//    GF::CircleDecrease<int16>(&PageMemory::PageLatest::currentSignal, 0, static_cast<int16>(Storage::AllDatas() - 1));
}

static void RotateSB_MemLast(int /*angle*/)
{
//    if (Storage::AllDatas() > 1)
//    {
//        Sound::RegulatorSwitchRotate();
//    }
//    if (Math::Sign(angle) > 0)
//    {
//        PressSB_MemLast_Next();
//    }
//    else
//    {
//        PressSB_MemLast_Prev();
//    }
}

static void FuncDrawingAdditionSPageMemoryLast()
{
    int width = 40;
    int height = 10;
    Region(width, height).Fill(Grid::Right() - width, Grid::TOP, Color::BACK);
    Primitives::Rectangle(width, height).Draw(Grid::Right() - width, Grid::TOP, Color::FILL);
    GF::Int2String(PageMemory::PageLatest::currentSignal + 1, false, 3).Draw(Grid::Right() - width + 2, Grid::TOP + 1);
    Text("/").Draw(Grid::Right() - width + 17, Grid::TOP + 1);

//    GF::Int2String(Storage::AllDatas(), false, 3).Draw(Grid::Right() - width + 23, Grid::TOP + 1);
}

void DrawSB_MemLast_IntEnter(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x40').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void DrawSB_MemLast_SaveToFlash(int x, int y)
{
    if (FDrive::isConnected)
    {
        Font::Set(TypeFont::UGO2);
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1);
        Font::Set(TypeFont::S8);
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

static void PressSB_MemLast_SaveToFlash()
{
    PageMemory::exitFromModeSetNameTo = RETURN_TO_LAST_MEM;
    Memory_SaveSignalToFlashDrive();
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

DEF_SMALL_BUTTON(sbMemLastPrev, PageMemory::PageLatest::self,
    "Предыдущий", "Previous",
    "Перейти к предыдущему сигналу",
    "Go to the previous signal",
    nullptr, PressSB_MemLast_Prev, DrawSB_MemLast_Prev, nullptr
)


DEF_SMALL_BUTTON(sbMemLastNext, PageMemory::PageLatest::self,
    "Следующий", "Next",
    "Перейти к следующему сигналу",
    "Go to the next signal",
    nullptr, PressSB_MemLast_Next, DrawSB_MemLast_Next, nullptr
)


void PressSB_MemLast_IntEnter()
{
    PageMemory::PageInternal::self->OpenAndSetItCurrent();
    MODE_WORK = ModeWork::MemInt;

//    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    exitFromIntToLast = true;
}

DEF_SMALL_BUTTON(sbMemLastIntEnter, PageMemory::PageLatest::self,
    "Внутр ЗУ", "Internal storage",
    "Нажмите эту кнопку, чтобы сохранить сигнал во внутреннем запоминающем устройстве",
    "Press this button to keep a signal in an internal memory",
    nullptr, PressSB_MemLast_IntEnter, DrawSB_MemLast_IntEnter, nullptr
)

DEF_SMALL_BUTTON(sbMemLastSaveToFlash, PageMemory::PageLatest::self,
    "Сохранить", "Save",
    "Кнопка становится доступна при присоединённом внешнем ЗУ. Позволяет сохранить сигнал на внешем ЗУ",
    "Click this button to save the signal on the external FLASH",
    nullptr, PressSB_MemLast_SaveToFlash, DrawSB_MemLast_SaveToFlash, nullptr
)

DEF_SMALL_BUTTON(sbSetNameSave, PageMemory::PageSetName::self,
    "Сохранить", "Save",
    "Сохранение на флеш под заданным именем",
    "Saving to flashdrive with the specified name",
    nullptr, PressSB_MemExtSetNameSave, DrawSB_MemExtSetNameSave, nullptr
)

void DrawSB_SetMask_Backspace(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void DrawSB_SetName_Backspace(int x, int y) //-V524
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::BACKSPACE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
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
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void PressSB_SetMask_Delete()
{
    FILE_NAME_MASK[0] = '\0';
}

void DrawSB_SetName_Delete(int x, int y) //-V524
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void PressSB_SetName_Delete()
{
    FILE_NAME[0] = '\0';
}

void DrawSB_SetMask_Insert(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::INSERT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
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
        FILE_NAME_MASK[size] = Tables::symbolsAlphaBet[index][0];
        FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // Для %nN - отдельный случай
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (FILE_NAME_MASK[size - 1] >= 0x30 && FILE_NAME_MASK[size - 1] <= 0x39) // Если ранее введено число
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
    Font::Set(TypeFont::UGO2);
    Char('\x26').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void PressSB_SetName_Insert()
{
    int size = static_cast<int>(std::strlen(FILE_NAME));
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        FILE_NAME[size] = Tables::symbolsAlphaBet[INDEX_SYMBOL][0];
        FILE_NAME[size + 1] = '\0';
    }
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    int8(*func[3])(int8 *, int8, int8) =
    {
        GF::CircleDecrease<int8>,
        GF::CircleDecrease<int8>,
        GF::CircleIncrease<int8>
    };

    Color::ResetFlash();
    if (INDEX_SYMBOL > maxIndex)
    {
        INDEX_SYMBOL = static_cast<int8>(maxIndex - 1);
    }
    func[Math::Sign(angle) + 1](&INDEX_SYMBOL, 0, static_cast<int8>(maxIndex - 1));
    Sound::RegulatorSwitchRotate();

}

static void OnMemExtSetMaskRegSet(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(Tables::symbolsAlphaBet) / 4);
}

DEF_SMALL_BUTTON(sbSetMaskBackspace, PageMemory::PageSetMask::self,
    "Backspace", "Backspace",
    "Удаляет последний введённый символ",
    "Deletes the last entered symbol",
    nullptr, PressSB_SetMask_Backspace, DrawSB_SetMask_Backspace, nullptr
)

DEF_SMALL_BUTTON(sbSetMaskDelete, PageMemory::PageSetMask::self,
    "Удалить", "Delete",
    "Удаляет все введённые символы",
    "Deletes all entered symbols",
    nullptr, PressSB_SetMask_Delete, DrawSB_SetMask_Delete, nullptr
)

DEF_SMALL_BUTTON(sbSetMaskInsert, PageMemory::PageSetMask::self,
    "Вставить", "Insert",
    "Вставляет выбранный символ",
    "Inserts the chosen symbol",
    nullptr, PressSB_SetMask_Insert, DrawSB_SetMask_Insert, nullptr
)


DEF_SMALL_BUTTON(sbSetNameBackspace, PageMemory::PageSetName::self,
    "Backspace", "Backspace",
    "Удаляет последний символ",
    "Delete the last character",
    nullptr, PressSB_SetName_Backspace, DrawSB_SetName_Backspace, nullptr
)

DEF_SMALL_BUTTON(sbSetNameDelete, PageMemory::PageSetName::self,
    "Удалить", "Delete",
    "Удаляет все введённые символы",
    "Deletes all entered characters",
    nullptr, PressSB_SetName_Delete, DrawSB_SetName_Delete, nullptr
)

DEF_SMALL_BUTTON(sbSetNameInsert, PageMemory::PageSetName::self,
    "Вставить", "Insert",
    "Вводит очередной символ",
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
    "Переход между каталогами и файлами",
    "The transition between the directories and files",
    nullptr, FM::PressTab, DrawSB_FM_Tab, nullptr
)

DEF_SMALL_BUTTON(sbFileManagerLevelDown, PageMemory::PageFileManager::self,
    "Войти", "Enter",
    "Переход в выбранный каталог",
    "Transition to the chosen catalog",
    nullptr, FM::PressLevelDown, DrawSB_FM_LevelDown, nullptr
)

DEF_SMALL_BUTTON(sbFileManagerLevelUp, PageMemory::PageFileManager::self,
    "Выйти", "Leave",
    "Переход в родительский каталог",
    "Transition to the parental catalog",
    nullptr, FM::PressLevelUp, DrawSB_FM_LevelUp, nullptr
)

void DrawSB_MemInt_SaveToIntMemory(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


static void SaveSignalToIntMemory()
{
    if (exitFromIntToLast)          // Если перешли во ВНУТР ЗУ из ПОСЛЕДНИЕ
    {
//        if  (Storage::dsLast != 0)
//        {                               // то сохраняем сигнал из последних
//            EPROM::SaveData(PageMemory::PageInternal::currentSignal, Storage::dsLast, Storage::dataLastA, Storage::dataLastB);
//            EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
//            Display::ShowWarningGood(Warning::SignalIsSaved);
//        }
    }
    else                                // Иначе сохраняем текущий сигнал
    {
//        if (Storage::set != 0)
//        {
//            EPROM::SaveData(PageMemory::PageInternal::currentSignal, Storage::set, Storage::dataA, Storage::dataB);
//            EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::set, &Storage::dataIntA, &Storage::dataIntB);
//            Display::ShowWarningGood(Warning::SignalIsSaved);
//        }
    }
}


void PressSB_MemInt_SaveToIntMemory()
{
    SaveSignalToIntMemory();
}

void DrawSB_MemInt_SaveToFlashDrive(int x, int y) //-V524
{
    if (FDrive::isConnected)
    {
        Font::Set(TypeFont::UGO2);
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1);
        Font::Set(TypeFont::S8);
    }
}

static void DrawMemoryWave(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 10;
    int width = 12;
    Region(width, 10).Fill(x, y, (num == PageMemory::PageInternal::currentSignal) ? Color::FLASH_10 : Color::BACK);
    Primitives::Rectangle(width, 10).Draw(x, y, Color::FILL);
    ((num == PageMemory::PageInternal::currentSignal) ? Color::FLASH_01 : Color::FILL).SetAsCurrent();
    if (exist)
    {
        GF::Int2String(num + 1, false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        Text("\x88").Draw(x + 3, y + 1);
    }
}

static void FuncAdditionDrawingSPageMemoryInt()
{
    // Теперь нарисуем состояние памяти

    bool exist[MAX_NUM_SAVED_WAVES] = {false};

    EPROM::GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave(i, exist[i]);
    }
}

void PressSB_MemInt_SaveToFlashDrive()
{
    PageMemory::exitFromModeSetNameTo = RETURN_TO_INT_MEM;
    Memory_SaveSignalToFlashDrive();
}

static void FuncOnRegSetMemInt(int delta)
{
    Sound::RegulatorSwitchRotate();

    if (delta < 0)
    {
        GF::CircleDecrease<int8>(&PageMemory::PageInternal::currentSignal, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        GF::CircleIncrease<int8>(&PageMemory::PageInternal::currentSignal, 0, MAX_NUM_SAVED_WAVES - 1);
    }

//    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    Color::ResetFlash();
}

static void DrawSB_MemInt_ShowSignalAllways_Yes(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x66').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

static void DrawSB_MemInt_ShowSignalAllways_No(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x68').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

static void DrawSB_MemInt_ShowSignalAlways(int x, int y)
{
    if (!PageMemory::PageInternal::showAlways)
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
    PageMemory::PageInternal::showAlways = !PageMemory::PageInternal::showAlways;
}

static const arrayHints hintsMemIntShowSignalAlways =
{
    {DrawSB_MemInt_ShowSignalAllways_Yes, "показывать выбранный сигнал из внутренней памяти поверх текущего",
                                          "to show the chosen signal from internal memory over the current"},
    {DrawSB_MemInt_ShowSignalAllways_No,  "сигнал из внутренней памяти виден только в режиме работы с внутренним запоминающим устройством",
                                          "the signal from internal memory is visible only in an operating mode with an internal memory"}
};

DEF_SMALL_BUTTON(sbMemIntShowSignalAlways, PageMemory::PageInternal::self,
    "Показывать всегда", "To show always",
    "Позволяет всегда показывать выбранный сохранённый сигнал поверх текущего",
    "Allows to show always the chosen kept signal over the current",
    nullptr, PressSB_MemInt_ShowSignalAlways, DrawSB_MemInt_ShowSignalAlways, &hintsMemIntShowSignalAlways
)


static void DrawSB_MemInt_ModeShow_Direct(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::MEM_INT_SHOW_DIRECT).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

static void DrawSB_MemInt_ModeShow_Saved(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::MEM_INT_SHOW_SAVED).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

static void DrawSB_MemInt_ModeShow_Both(int x, int y)
{
    ++y;

    Primitives::Rectangle(4, 7).Draw(x + 2, y + 5);

    Primitives::Rectangle(4, 4).Draw(x + 8, y + 8);
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
    GF::CircleIncrease<int8>((int8*)&MODE_SHOW_MEMINT, 0, 2);
}

static const arrayHints hintsMemIntModeShow =
{
    { DrawSB_MemInt_ModeShow_Direct, "на дисплее текущий сигнал",     "on the display current signal" },
    { DrawSB_MemInt_ModeShow_Saved,  "на дисплее сохранённый сигнал", "on the display the kept signal" },
    { DrawSB_MemInt_ModeShow_Both,   "на дисплее оба сигнала",        "on the display the both signals" }
};

DEF_SMALL_BUTTON(sbMemIntModeShow, PageMemory::PageInternal::self,
    "Вид сигнала", "Type of a signal",
    "Показывать записанный или текущий сигнал в режиме ВНУТР ЗУ",
    "Show recorded or current signal in mode Internal Memory",
    nullptr, PressSB_MemInt_ModeShow, DrawSB_MemInt_ModeShow, &hintsMemIntModeShow
)


static void PressSB_MemInt_Delete()
{
    EPROM::DeleteData(PageMemory::PageInternal::currentSignal);
}

static void DrawSB_MemInt_Delete(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

DEF_SMALL_BUTTON(sbMemIntDelete, PageMemory::PageInternal::self,
    "Удалить сигнал", "Delete signal",
    "Удалить сигнал",
    "Delete signal",
    nullptr, PressSB_MemInt_Delete, DrawSB_MemInt_Delete, nullptr
)


DEF_SMALL_BUTTON(sbMemIntSave, PageMemory::PageInternal::self,
    "Сохранить", "Save",
    "Сохранить сигнал во внутреннем запоминующем устройстве",
    "To keep a signal in an internal memory",
    nullptr, PressSB_MemInt_SaveToIntMemory, DrawSB_MemInt_SaveToIntMemory, nullptr
)

DEF_SMALL_BUTTON(sbMemIntSaveToFlash, PageMemory::PageInternal::self,
    "Сохранить", "Save",
    "Сохраняет сигнал на флешку",
    "Save signal to flash drive",
    nullptr, PressSB_MemInt_SaveToFlashDrive, DrawSB_MemInt_SaveToFlashDrive, nullptr
)

void PressSB_MemInt_Exit()
{
//    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    if (exitFromIntToLast)
    {
        PageMemory::PageLatest::self->OpenAndSetItCurrent();
        MODE_WORK = ModeWork::Latest;
        exitFromIntToLast = false;
        Menu::needClosePageSB = false;
    }
    else
    {
        MODE_WORK = ModeWork::Direct;
    }
}

DEF_SMALL_BUTTON(sbExitSetName, PageMemory::PageSetName::self,     // Кнопк для выхода из режима задания имени сохраняемому сигналу. Одновременно кнопка отказа от сохранения
    EXIT_RU, EXIT_EN,
    "Отказ от сохранения",
    "Failure to save",
    nullptr, PressSB_SetName_Exit, DrawSB_Exit, nullptr
)

DEF_SMALL_BUTTON(sbExitMemInt, PageMemory::PageInternal::self,    // Кнопка для выхода из режима малых кнопок.
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_MemInt_Exit, DrawSB_Exit, nullptr
)


// Нажатие ПАМЯТЬ - ВНЕШН ЗУ - Маска
void OnPressMemoryExtMask()
{
    PageMemory::PageSetMask::self->OpenAndSetItCurrent();
    Display::SetAddDrawFunction(DrawSetMask);
}

static bool FuncActiveMemoryNumPoinst()
{
    return !PeackDetMode::IsEnabled();
}

// ПАМЯТЬ - Точки
int8 temp = 0;

DEF_CHOICE_3(mcMemoryNumPoints, PageMemory::self,
    "Точки", "Points"
    ,
    "Выбор количества отсчётов для сохраняемых сигналов. "
    "При увеличении количества отсчётов уменьшается количество сохранённых в памяти сигналов."
    ,
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases."
    ,
    "281",  "281",
    "512",  "512",
    "1024", "1024",
    set.memory.enum_points_fpga, FuncActiveMemoryNumPoinst, ChangeC_Memory_NumPoints, nullptr
)

DEF_CHOICE_2(mcMemoryExtName, PageMemory::PageExternal::self,
    "Имя файла", "File name"
    ,
    "Задаёт режим наименования файлов при сохранении на внешний накопитель:\n"
    "\"По маске\" - файлы именуются автоматически по заранее введённой маске (след. пункт меню),\n"
    "\"Вручную\" - каждый раз имя файла нужно задавать вручную"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "По маске", "Mask",
    "Вручную",  "Manually",
    FILE_NAMING_MODE, nullptr, nullptr, nullptr
)

    
// ПАМЯТЬ - ВНЕШН ЗУ - Автоподключение
DEF_CHOICE_2(mcMemoryExtAutoConnectFlash, PageMemory::PageExternal::self,
    "Автоподкл.", "AutoConnect",
    "Eсли \"Вкл\", при подключении внешнего накопителя происходит автоматический переход на страницу ПАМЯТЬ - Внешн ЗУ",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    FLASH_AUTOCONNECT, nullptr, nullptr, nullptr
)


// ПАМЯТЬ - ВНЕШН ЗУ - Реж кн ПАМЯТЬ
DEF_CHOICE_2(mcMemoryExtModeBtnMemory, PageMemory::PageExternal::self,
    "Реж кн ПАМЯТЬ", "Mode btn MEMORY",
    "",
    "",
    "Меню",       "Menu",
    "Сохранение", "Save",
    MODE_BTN_MEMORY, nullptr, nullptr, nullptr
)


// ПАМЯТЬ - ВНЕШН ЗУ - Сохранять как
DEF_CHOICE_2(mcMemoryExtModeSave, PageMemory::PageExternal::self,
    "Сохранять как", "Save as"
    ,
    "Если выбран вариант \"Изображение\", сигнал будет сохранён в текущем каталоге в графическом файле с расширением BMP\n"
    "Если выбран вариант \"Текст\", сигнал будет сохранён в текущем каталоге в текстовом виде в файле с раширением TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension BMP\n"
    "If you select \"Text\", the signal will be stored in the current directory as a text file with the extension TXT"
    ,
    "Изображение", "Image",
    "Текст",       "Text",
    MODE_SAVE_SIGNAL, nullptr, nullptr, nullptr
)

    
void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::TOP + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Primitives::Rectangle(width, height).Draw(x0, y0, Color::FILL);
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

    int x = Text(FILE_NAME).Draw(x0 + deltaX, y0 + 65, Color::FILL);
    Region(5, 8).Fill(x, y0 + 65, Color::FLASH_10);
}

static void DrawFileMask(int x, int y)
{
    char *ch = FILE_NAME_MASK;

    Color::FILL.SetAsCurrent();
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
                x = Text(Tables::symbolsAlphaBet[*ch + 0x40]).Draw(x, y);
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

    Primitives::Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // Рисуем большие буквы английского алфавита
    while(Tables::symbolsAlphaBet[index][0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }
    
    // Теперь рисуем цифры и пробел
    position = 0;
    while(Tables::symbolsAlphaBet[index][0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // Теперь рисуем малые буквы алфавита
    position = 0;
    while(Tables::symbolsAlphaBet[index][0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // Теперь рисуем спецсимволы
    position = 0;
    while (index < (sizeof(Tables::symbolsAlphaBet) / 4))
    {
        Tables::DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static const char* strings[] =
    {
        "%y - год, %m - месяц, %d - день",
        "%H - часы, %M - минуты, %S - секунды",
        "%nN - порядковый номер, где",
        "n - минимальное количество знаков для N"
    };

    deltaY--;
    Color::FILL.SetAsCurrent();
    for(int i = 0; i < sizeof(strings) / 4; i++)
    {
        Text(strings[i]).Draw(x0 + deltaX, y0 + 100 + deltaY * i);
    }
}

void Memory_SaveSignalToFlashDrive()
{
    if (FDrive::isConnected)
    {
        if (FILE_NAMING_MODE_IS_HAND)
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

static void PressSB_MemLast_Exit()
{
    MODE_WORK = ModeWork::Direct;
    if (runningFPGAbeforeSmallButtons)
    {
        FPGA::Start();
        runningFPGAbeforeSmallButtons = false;
    }
    Display::RemoveAddDrawFunction();
}

// Нажатие ПАМЯТЬ - Последние.
void OnPressMemoryLatest()
{
    PageMemory::PageLatest::currentSignal = 0;
    runningFPGAbeforeSmallButtons = FPGA::IsRunning();
    FPGA::Stop(false);
    MODE_WORK = ModeWork::Latest;
}

DEF_SMALL_BUTTON(sbExitMemLast, PageMemory::PageLatest::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_MemLast_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_6(pageLatest, PageMemory::self, NamePage::SB_MemLatest,
    "ПОСЛЕДНИЕ", "LATEST",
    "Переход в режим работы с последними полученными сигналами",
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

DEF_SMALL_BUTTON(sbExitSetMask, PageMemory::PageSetMask::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_SetMask_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_6(pageSetMask, PageMemory::PageExternal::self, NamePage::SB_MemExtSetMask,
    "МАСКА", "MASK",
    "Режим ввода маски для автоматического именования файлов",
    "Input mode mask for automatic file naming",
    sbExitSetMask,
    sbSetMaskDelete,
    Item::empty,
    Item::empty,
    sbSetMaskBackspace,
    sbSetMaskInsert,
    IsActiveMemoryExtSetMask, OnPressMemoryExtMask, nullptr, OnMemExtSetMaskRegSet
)

void OnPressMemoryExtFileManager()
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
    "КАТАЛОГ", "DIRECTORY",
    "Открывает доступ к файловой системе подключенного накопителя",
    "Provides access to the file system of the connected drive",
    sbExitFileManager,
    sbFileManagerTab,
    Item::empty,
    Item::empty,
    sbFileManagerLevelUp,
    sbFileManagerLevelDown,
    FuncOfActiveExtMemFolder, OnPressMemoryExtFileManager, EmptyFuncVV, FM::RotateRegSet
);

DEF_PAGE_6(pageExternal, PageMemory::self, NamePage::MemoryExt,
    "ВНЕШН ЗУ", "EXT STORAGE",
    "Работа с внешним запоминающим устройством.",
    "Work with external storage device.",
    *PageMemory::PageFileManager::self,
    mcMemoryExtName,
    *PageMemory::PageSetMask::self,
    mcMemoryExtModeSave,
    mcMemoryExtModeBtnMemory,
    mcMemoryExtAutoConnectFlash,
    nullptr, nullptr, nullptr, nullptr
);

void OnPressMemoryInt()
{
    PageMemory::PageInternal::self->OpenAndSetItCurrent();
    MODE_WORK = ModeWork::MemInt;

//    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
}

DEF_PAGE_6(pageInternal, PageMemory::self, NamePage::SB_MemInt,
    "ВНУТР ЗУ", "INT STORAGE",
    "Переход в режим работы с внутренней памятью",
    "Transition to an operating mode with internal memory",
    sbExitMemInt,
    sbMemIntShowSignalAlways,
    sbMemIntModeShow,
    sbMemIntDelete,
    sbMemIntSave,
    sbMemIntSaveToFlash,
    nullptr, OnPressMemoryInt, FuncAdditionDrawingSPageMemoryInt, FuncOnRegSetMemInt
);

// Страница вызывается при выбранном ручном режиме задания имени файла перед сохранением на флешку ///////////////
static void OnMemExtSetNameRegSet(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(Tables::symbolsAlphaBet) / 4 - 7);
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
const Page *PageMemory::PageLatest::self = &pageLatest;
const Page *PageMemory::PageInternal::self = &pageInternal;
const Page *PageMemory::PageExternal::self = &pageExternal;
const Page *PageMemory::PageSetMask::self = &pageSetMask;
const Page *PageMemory::PageSetName::self = &pageSetName;
const Page *PageMemory::PageFileManager::self = &pageFileManager;
