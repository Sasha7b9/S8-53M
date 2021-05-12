// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcMapping, PageDisplay::self,
    "Отображение", "View",
    "Задаёт режим отображения сигнала.",
    "Sets the display mode signal.",
    "Вектор", "Vector",
    "Точки", "Points",
    set.display.mode_draw_signal, nullptr, nullptr, nullptr
)













static bool IsActive_MinMax()
{
    return (set.time.base > TBase::_50ns);
}

static void OnChanged_MinMax(bool)
{
}

DEF_CHOICE_REG_8(mcMinMax, PageDisplay::self,
    "Мин Макс", "Min Max",
    "Задаёт количество последних измерений, по которым строятся ограничительные линии, огибающие минимумы и максимумы измерений.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under construction.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    set.display.enum_min_max, IsActive_MinMax, OnChanged_MinMax, nullptr
)

DEF_CHOICE_REG_10(mcSmoothing, PageDisplay::self,
    "Сглаживание", "Smoothing",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 точки", "2 points",
    "3 точки", "3 points",
    "4 точки", "4 points",
    "5 точек", "5 points",
    "6 точек", "6 points",
    "7 точек", "7 points",
    "8 точек", "8 points",
    "9 точек", "9 points",
    "10 точек", "10 points",
    set.display.smoothing, nullptr, nullptr, nullptr
)

void PageDisplay::OnChanged_RefreshFPS(bool)
{
    FPGA::SetNumSignalsInSec(set.display.enum_fps.NumSignalsInS());
}

DEF_CHOICE_5(mcRefreshFPS, PageDisplay::self,
    "Частота обновл", "Refresh rate",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5", "5",
    "2", "2",
    "1", "1",
    set.display.enum_fps, nullptr, PageDisplay::OnChanged_RefreshFPS, nullptr
)

DEF_CHOICE_2(mcTypeShift, PageDisplay::self,
    "Смещение", "Offset",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется напряжение смещения.\n2. \"Деления\" - сохраняется положение смещения на экране.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of the offset on the screen.",
    "Напряжение", "Voltage",
    "Деления", "Divisions",
    set.display.linking_rshift, nullptr, nullptr, nullptr
)

DEF_PAGE_9(pageDisplay, PageMain::self, NamePage::Display,
    "ДИСПЛЕЙ", "DISPLAY",
    "Содержит настройки отображения дисплея.",
    "Contains settings of display of the Display::",
    mcMapping,
    *PageDisplay::PageAccumulation::self,
    *PageDisplay::PageAveraging::self,
    mcMinMax,
    mcSmoothing,
    mcRefreshFPS,
    *PageDisplay::PageGrid::self,
    mcTypeShift,
    *PageDisplay::PageSettings::self,
    nullptr, nullptr, nullptr, nullptr
)

void PageDisplay::OnPress_Accumulation_Clear()
{
    Display::Redraw();
}



DEF_CHOICE_2(mcSettings_Colors_Scheme, PageDisplay::PageSettings::PageColors::self,
    "Цветовая схема", "Color scheme",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2",
    set.service.colorScheme, nullptr, nullptr, nullptr
)

static ColorType colorT1 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_A.index };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelA, PageDisplay::PageSettings::PageColors::self,
    "Канал 1", "Channel 1",
    "",
    "",
    colorT1, nullptr
)

static ColorType colorT2 = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::DATA_B.index };

DEF_GOVERNOR_COLOR(mgcSettings_Colors_ChannelB, PageDisplay::PageSettings::PageColors::self,
    "Канал 2", "Channel 2",
    "",
    "",
    colorT2, nullptr
)

DEF_GOVERNOR_COLOR(mgcSettings_Colors_Grid, PageDisplay::PageSettings::PageColors::self,
    "Сетка", "Grid",
    "Устанавливает цвет сетки",
    "Sets the grid color",
    PageDisplay::PageGrid::typeGrid, nullptr
)

DEF_PAGE_4(pageColors, PageDisplay::PageSettings::self, NamePage::ServiceDisplayColors,
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    mcSettings_Colors_Scheme,
    mgcSettings_Colors_ChannelA,
    mgcSettings_Colors_ChannelB,
    mgcSettings_Colors_Grid,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDisplay::self = &pageDisplay;
const Page *PageDisplay::PageSettings::PageColors::self = &pageColors;
