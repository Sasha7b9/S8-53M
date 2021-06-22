// 2021/05/12 13:34:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


static bool runningFPGAbeforeSmallButtons = false;      // Здесь сохраняется информация о том, работала ли ПЛИС перед
                                                        // переходом в режим работы с памятью

static void PressSB_Exit()
{
    set.memory.mode_work = ModeWork::Direct;
    if (runningFPGAbeforeSmallButtons)
    {
        FPGA::Start();
        runningFPGAbeforeSmallButtons = false;
    }
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(sbExit, PageMemory::PageLatest::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void PressSB_Next()
{
    //    Math::CircleIncrease<int16>(&PageMemory::PageLatest::currentSignal, 0, (int16)(Storage::AllDatas() - 1));
}


static void DrawSB_Next(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x64').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbNext, PageMemory::PageLatest::self,
    "Следующий", "Next",
    "Перейти к следующему сигналу",
    "Go to the next signal",
    nullptr, PressSB_Next, DrawSB_Next, nullptr
)


static void PressSB_Prev()
{
    //    Math::CircleDecrease<int16>(&PageMemory::PageLatest::currentSignal, 0, (int16)(Storage::AllDatas() - 1));
}


static void DrawSB_Prev(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x20').Draw4SymbolsInRect(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbPrev, PageMemory::PageLatest::self,
    "Предыдущий", "Previous",
    "Перейти к предыдущему сигналу",
    "Go to the previous signal",
    nullptr, PressSB_Prev, DrawSB_Prev, nullptr
)


static void PressSB_EnterInt()
{
    PageMemory::PageInternal::self->OpenAndSetItCurrent();
    set.memory.mode_work = ModeWork::MemInt;

    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    PageMemory::exitFromIntToLast = true;
}


static void DrawSB_EnterInt(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x40').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sbEnterInt, PageMemory::PageLatest::self,
    "Внутр ЗУ", "Internal storage",
    "Нажмите эту кнопку, чтобы сохранить сигнал во внутреннем запоминающем устройстве",
    "Press this key to keep a signal in an internal memory",
    nullptr, PressSB_EnterInt, DrawSB_EnterInt, nullptr
)


static void PressSB_SaveToFlash()
{
    PageMemory::exitFromModeSetNameTo = RETURN_TO_LAST_MEM;
    PageMemory::PageExternal::SaveSignalToFlashDrive();
}


static void DrawSB_SaveToFlash(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Font::Set(TypeFont::UGO2);
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1);
        Font::Set(TypeFont::S8);
    }
}


DEF_SMALL_BUTTON(sbSaveToFlash, PageMemory::PageLatest::self,
    "Сохранить", "Save",
    "Кнопка становится доступна при присоединённом внешнем ЗУ. Позволяет сохранить сигнал на внешем ЗУ",
    "Click this key to save the signal on the external FLASH",
    nullptr, PressSB_SaveToFlash, DrawSB_SaveToFlash, nullptr
)


static void OnPress()
{
    PageMemory::PageLatest::currentSignal = 0;
    runningFPGAbeforeSmallButtons = FPGA::IsRunning();
    FPGA::Stop();
    set.memory.mode_work = ModeWork::Latest;
}


static void FuncDrawingAddition()
{
    int width = 40;
    int height = 10;
    Region(width, height).Fill(Grid::Right() - width, Grid::TOP, Color::BACK);
    Rectangle(width, height).Draw(Grid::Right() - width, Grid::TOP, Color::FILL);
    Int(PageMemory::PageLatest::currentSignal + 1).ToText(false, 3).Draw(Grid::Right() - width + 2, Grid::TOP + 1);
    Text("/").Draw(Grid::Right() - width + 17, Grid::TOP + 1);

    //    GF::Int2String(Storage::AllDatas(), false, 3).Draw(Grid::Right() - width + 23, Grid::TOP + 1);
}


static void OnRegSet(int /*angle*/)
{
    //    if (Storage::AllDatas() > 1)
    //    {
    //        Sound::RegulatorSwitchRotate();
    //    }
    //    if (Math::Sign(angle) > 0)
    //    {
    //        PressSB_Next();
    //    }
    //    else
    //    {
    //        PressSB_Prev();
    //    }
}


DEF_PAGE_6(pageLatest, PageMemory::self, NamePage::SB_MemLatest,
    "ПОСЛЕДНИЕ", "LATEST",
    "Переход в режим работы с последними полученными сигналами",
    "Transition to an operating mode with the last received signals",
    sbExit,
    Item::empty,
    sbNext,
    sbPrev,
    sbEnterInt,
    sbSaveToFlash,
    nullptr, OnPress, FuncDrawingAddition, OnRegSet
)

const Page *PageMemory::PageLatest::self = &pageLatest;
