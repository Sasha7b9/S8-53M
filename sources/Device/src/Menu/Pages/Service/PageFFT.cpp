// 2021/05/12 15:44:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cMath_FFT_Enable, PageService::PageMath::PageFFT::self,
    "Отображение", "Display",
    "Включает и выключает отображение спектра",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.math.fft.enabled, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(cMath_FFT_Scale, PageService::PageMath::PageFFT::self,
    "Шкала", "Scale",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "Логарифм", "Log",
    "Линейная", "Linear",
    set.math.fft.scale, nullptr, nullptr, nullptr
)


DEF_CHOICE_3(cMath_FFT_Source, PageService::PageMath::PageFFT::self,
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Selecting the source for the calculation of the spectrum",
    "Канал 1", "Channel 1",
    "Канал 2", "Channel 2",
    "Канал 1 + 2", "Channel 1 + 2",
    set.math.fft.source, nullptr, nullptr, nullptr
)


DEF_CHOICE_4(cMath_FFT_Window, PageService::PageMath::PageFFT::self,
    "Окно", "Window",
    "Задаёт окно для расчёта спектра",
    "Sets the window to calculate the spectrum",
    "Прямоугольн", "Rectangle",
    "Хэмминга", "Hamming",
    "Блэкмена", "Blackman",
    "Ханна", "Hann",
    set.math.fft.window, nullptr, nullptr, nullptr
)


static bool IsActive_Math_FFT_Limit()
{
    return set.math.fft.scale.IsLog();
}


DEF_CHOICE_3(cMath_FFT_Limit, PageService::PageMath::PageFFT::self,
    "Диапазон", "Range",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "Here you can set the limit of monitoring the power spectrum",
    "-40дБ", "-40dB",
    "-60дБ", "-60dB",
    "-80дБ", "-80dB",
    set.math.fft.max_DB, IsActive_Math_FFT_Limit, nullptr, nullptr
)


static bool IsActive_Math_FFT()
{
    return ModeDrawMath::IsDisabled();
}


static void OnPress_Math_FFT()
{
    if (!IsActive_Math_FFT())
    {
        Warnings::ShowWarningBad(Warning::ImpossibleEnableFFT);
    }
}


DEF_PAGE_6(pageFFT, PageService::PageMath::self, NamePage::MathFFT,
    "СПЕКТР", "SPECTRUM",
    "Отображение спектра входного сигнала",
    "Mapping the input signal spectrum",
    cMath_FFT_Enable,                                   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Отображение
    cMath_FFT_Scale,                                    // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Шкала
    cMath_FFT_Source,                                   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Источник
    cMath_FFT_Window,                                   // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Окно
    PageService::PageMath::PageFFT::PageCursors::self,  // СЕРВИС - МАТЕМАТИКА - СПЕКТР - КУРСОРЫ
    cMath_FFT_Limit,                                    // СЕРВИС - МАТЕМАТИКА - СПЕКТР - Диапазон
    IsActive_Math_FFT, OnPress_Math_FFT, nullptr, nullptr
)

const Page *PageService::PageMath::PageFFT::self = &pageFFT;
