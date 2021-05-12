// 2021/05/12 13:51:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "common/Utils/Math_.h"
#include "FDrive/FDrive.h"
#include "Menu/Menu.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


void PressSB_MemInt_Exit()
{
    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    if (PageMemory::exitFromIntToLast)
    {
        PageMemory::PageLatest::self->OpenAndSetItCurrent();
        set.memory.mode_work = ModeWork::Latest;
        PageMemory::exitFromIntToLast = false;
        Menu::needClosePageSB = false;
    }
    else
    {
        set.memory.mode_work = ModeWork::Direct;
    }
}


DEF_SMALL_BUTTON(sbExitMemInt, PageMemory::PageInternal::self,    // Кнопка для выхода из режима малых кнопок.
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_MemInt_Exit, DrawSB_Exit, nullptr
)


static void PressSB_MemInt_ShowSignalAlways()
{
    PageMemory::PageInternal::showAlways = !PageMemory::PageInternal::showAlways;
}


static void DrawSB_MemInt_ShowSignalAllways_No(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x68').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


static void DrawSB_MemInt_ShowSignalAllways_Yes(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x66').Draw4SymbolsInRect(x + 2, y + 1);
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


static void PressSB_MemInt_ModeShow()
{
    Math::CircleIncrease<int8>((int8 *)&set.memory.mode_show_int_mem, 0, 2);
}


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
    ModeShowIntMem &mode = set.memory.mode_show_int_mem;

    switch (mode.value)
    {
    case ModeShowIntMem::Direct:    DrawSB_MemInt_ModeShow_Direct(x, y);       break;
    case ModeShowIntMem::Saved:     DrawSB_MemInt_ModeShow_Saved(x, y);        break;
    case ModeShowIntMem::Both:      DrawSB_MemInt_ModeShow_Both(x, y);         break;
    }
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
    ROM::Data::Erase((uint)PageMemory::PageInternal::currentSignal);
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


static void SaveSignalToIntMemory()
{
    if (PageMemory::exitFromIntToLast)          // Если перешли во ВНУТР ЗУ из ПОСЛЕДНИЕ
    {
        //        if  (Storage::dsLast != 0)
        //        {                               // то сохраняем сигнал из последних
        //            EPROM::SaveData(PageMemory::PageInternal::currentSignal, Storage::dsLast, Storage::dataLastA, Storage::dataLastB);
        //            EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
        //            Warnings::ShowWarningGood(Warning::SignalIsSaved);
        //        }
    }
    else                                // Иначе сохраняем текущий сигнал
    {
        //        if (Storage::set != 0)
        //        {
        //            EPROM::SaveData(PageMemory::PageInternal::currentSignal, Storage::set, Storage::dataA, Storage::dataB);
        //            EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::set, &Storage::dataIntA, &Storage::dataIntB);
        //            Warnings::ShowWarningGood(Warning::SignalIsSaved);
        //        }
    }
}


void PressSB_MemInt_SaveToIntMemory()
{
    SaveSignalToIntMemory();
}


void DrawSB_MemInt_SaveToIntMemory(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char(Symbol::UGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbMemIntSave, PageMemory::PageInternal::self,
    "Сохранить", "Save",
    "Сохранить сигнал во внутреннем запоминующем устройстве",
    "To keep a signal in an internal memory",
    nullptr, PressSB_MemInt_SaveToIntMemory, DrawSB_MemInt_SaveToIntMemory, nullptr
)


void PressSB_MemInt_SaveToFlashDrive()
{
    PageMemory::exitFromModeSetNameTo = RETURN_TO_INT_MEM;
    PageMemory::PageExternal::SaveSignalToFlashDrive();
}


void DrawSB_MemInt_SaveToFlashDrive(int x, int y)
{
    if (FDrive::isConnected)
    {
        Font::Set(TypeFont::UGO2);
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1);
        Font::Set(TypeFont::S8);
    }
}


DEF_SMALL_BUTTON(sbMemIntSaveToFlash, PageMemory::PageInternal::self,
    "Сохранить", "Save",
    "Сохраняет сигнал на флешку",
    "Save signal to flash drive",
    nullptr, PressSB_MemInt_SaveToFlashDrive, DrawSB_MemInt_SaveToFlashDrive, nullptr
)


void OnPressMemoryInt()
{
    PageMemory::PageInternal::self->OpenAndSetItCurrent();
    set.memory.mode_work = ModeWork::MemInt;

    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
}


static void DrawMemoryWave(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 10;
    int width = 12;
    Region(width, 10).Fill(x, y, (num == PageMemory::PageInternal::currentSignal) ? Color::FLASH_10 : Color::BACK);
    Rectangle(width, 10).Draw(x, y, Color::FILL);
    ((num == PageMemory::PageInternal::currentSignal) ? Color::FLASH_01 : Color::FILL).SetAsCurrent();
    if (exist)
    {
        Int(num + 1).ToText(false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        Text("\x88").Draw(x + 3, y + 1);
    }
}


static void FuncAdditionDrawingSPageMemoryInt()
{
    // Теперь нарисуем состояние памяти

    bool exist[ROM::Data::MAX_NUM_SAVED_WAVES] = { false };

    ROM::Data::GetInfo(exist);

    for (uint i = 0; i < ROM::Data::MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave((int)i, exist[i]);
    }
}


static void FuncOnRegSetMemInt(int delta)
{
    Sound::RegulatorSwitchRotate();

    if (delta < 0)
    {
        Math::CircleDecrease<int8>(&PageMemory::PageInternal::currentSignal, 0, ROM::Data::MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        Math::CircleIncrease<int8>(&PageMemory::PageInternal::currentSignal, 0, ROM::Data::MAX_NUM_SAVED_WAVES - 1);
    }

    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    Color::ResetFlash();
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


const Page *PageMemory::PageInternal::self = &pageInternal;
