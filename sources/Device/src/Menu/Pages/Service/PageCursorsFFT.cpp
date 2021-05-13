// 2021/05/12 15:50:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsCommon.h"


static void OnPress_Exit()
{
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(cExit, PageService::PageMath::PageFFT::PageCursors::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Exit, DrawSB_Exit, nullptr
)


static void OnPress_Source()
{
    set.math.fft.cursor = (uint8)((set.math.fft.cursor + 1) % 2);
}


static void Draw_Source(int x, int y)
{
    Text((set.math.fft.cursor == 0) ? "1" : "2").Draw(x + 7, y + 5);
}


DEF_SMALL_BUTTON(cSource, PageService::PageMath::PageFFT::PageCursors::self,
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Source choice for calculation of a range",
    nullptr, OnPress_Source, Draw_Source, nullptr
)


static void OnRegSet(int angle)
{
    set.math.fft.pos_cur[set.math.fft.cursor] += (uint8)angle;
    Sound::RegulatorShiftRotate();
}


static bool IsActive()
{
    return set.math.fft.enabled;
}


DEF_PAGE_6(pageCursorsFFT, PageService::PageMath::PageFFT::self, NamePage::SB_MathCursorsFFT,
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    cExit,
    cSource,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    IsActive, nullptr, nullptr, OnRegSet
)

const Page *PageService::PageMath::PageFFT::PageCursors::self = &pageCursorsFFT;
