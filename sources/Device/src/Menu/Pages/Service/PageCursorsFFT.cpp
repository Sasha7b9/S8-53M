// 2021/05/12 15:50:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void OnPress_Math_FFT_Cursors_Exit()
{
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(cMath_FFT_Cursors_Exit, PageService::PageMath::PageFFT::PageCursors::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Math_FFT_Cursors_Exit, DrawSB_Exit, nullptr
)


static void OnPress_Math_FFT_Cursors_Source()
{
    set.math.fft.cursor = (uint8)((set.math.fft.cursor + 1) % 2);
}


static void Draw_Math_FFT_Cursors_Source(int x, int y)
{
    Text((set.math.fft.cursor == 0) ? "1" : "2").Draw(x + 7, y + 5);
}


DEF_SMALL_BUTTON(cMath_FFT_Cursors_Source, PageService::PageMath::PageFFT::PageCursors::self,
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Source choice for calculation of a range",
    nullptr, OnPress_Math_FFT_Cursors_Source, Draw_Math_FFT_Cursors_Source, nullptr
)


static void OnRegSet_Math_FFT_Cursors(int angle)
{
    set.math.fft.pos_cur[set.math.fft.cursor] += (uint8)angle;
    Sound::RegulatorShiftRotate();
}


static bool IsActive_Math_FFT_Cursors()
{
    return set.math.fft.enabled;
}


DEF_PAGE_6(pageCursorsFFT, PageService::PageMath::PageFFT::self, NamePage::SB_MathCursorsFFT,
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    cMath_FFT_Cursors_Exit,     // СЕРВИС - МАТЕМАТИКА - СПЕКТР - КУРСОРЫ - Выход
    cMath_FFT_Cursors_Source,   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - КУРСОРЫ - Источник
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    IsActive_Math_FFT_Cursors, nullptr, nullptr, OnRegSet_Math_FFT_Cursors
)

const Page *PageService::PageMath::PageFFT::PageCursors::self = &pageCursorsFFT;
