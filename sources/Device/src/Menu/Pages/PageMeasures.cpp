#include "defines.h"
#include "common/Utils/Math_c.h"
#include "common/Display/Text_c.h"
#include "common/Display/Font/Font_c.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Measures.h"

bool PageMeasures::choiceMeasuresIsActive = false;

// Каким курсором в данный момент происходит управление
static CursCntrl::E GetMeasuresCursCntrlActive() //-V2506
{
    if (MEAS_CURS_ACTIVE_IS_T)
    {
        return MEAS_CURS_CNTRL_T;
    }
    return MEAS_CURS_CNTRL_U;
}

void DrawSB_MeasTune_Settings(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x62').Draw4SymbolsInRect(x + 2, y + 1);
    Font::Set(TypeFont::S8);
}

void PressSB_MeasTune_Settings()
{
    Measure::ShorPressOnSmallButtonSettings();
}

void DrawSB_MeasTune_Markers(int x, int y)
{
    Font::Set(TypeFont::UGO2);
    Char('\x60').Draw(x + 2, y + 2);
    Font::Set(TypeFont::S8);
}

void PressSB_MeasTune_Markers()
{
    Measure::ShortPressOnSmallButonMarker();
}

void RotateRegMeasureSetField(int angle)
{
    CursCntrl::E cursCntrl = GetMeasuresCursCntrlActive();
    CursActive::E cursActive = MEAS_CURS_ACTIVE;

    int sign = Math::Sign(angle);

    if (cursCntrl == CursCntrl::_1 || cursCntrl == CursCntrl::_1_2)
    {
        if (cursActive == CursActive::T)
        {
            LIMITATION(MEAS_POS_CUR_T0, static_cast<int16>(MEAS_POS_CUR_T0 - sign), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 1)); //-V2516
        }
        else
        {
            LIMITATION(MEAS_POS_CUR_U0, static_cast<int16>(MEAS_POS_CUR_U0 + sign), 0, 200); //-V2516
        }
    }
    if (cursCntrl == CursCntrl::_2 || cursCntrl == CursCntrl::_1_2)
    {
        if (cursActive == CursActive::T)
        {
            LIMITATION(MEAS_POS_CUR_T1, static_cast<int16>(MEAS_POS_CUR_T1 - sign), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 1)); //-V2516
        }
        else
        {
            LIMITATION(MEAS_POS_CUR_U1, static_cast<int16>(MEAS_POS_CUR_U1 + sign), 0, 200); //-V2516
        }
    }
}

DEF_SMALL_BUTTON( sbMeasTuneSettings, PageMeasures::PageTune::self,
    "Настройка", "Setup",
    "Позволяет выбрать необходимые измерения",
    "Allows to choose necessary measurements",
    nullptr, PressSB_MeasTune_Settings, DrawSB_MeasTune_Settings, nullptr
)

DEF_SMALL_BUTTON( sbMeasTuneMarkers, PageMeasures::PageTune::self,                 // Включение / отключение маркера для режима измерений.
    "Маркер", "Marker",
    "Позволяет установить маркеры для визуального контроля измерений",
    "Allows to establish markers for visual control of measurements",
    nullptr, PressSB_MeasTune_Markers, DrawSB_MeasTune_Markers, nullptr
)

bool IsActiveChoiceMeasuresNumber()
{
    return SHOW_MEASURES;
}

bool IsActiveChoiceMeasuresChannels()
{
    return SHOW_MEASURES;
}

bool IsActivePageMeasuresFields()
{
    return SHOW_MEASURES;
}

bool IsActiveChoiceMeasuresSignal()
{
    return SHOW_MEASURES;
}

bool IsActiveButtonMeasuresTune()
{
    return SHOW_MEASURES;
}

bool IsActiveButtonMeasuresFieldSet()
{
    return MEAS_FIELD_IS_HAND;
}

DEF_CHOICE_7( mcMeasuresNumber, PageMeasures::self,
    "Количество", "Number"
    ,
    "Устанавливает максимальное количество выводимых измерений:\n"
    "\"1\" - одно измерение\n"
    "\"2\" - два измерения\n"
    "\"1х5\" - 1 строка с пятью измерениями\n"
    "\"2х5\" - 2 строки с пятью измерениями в каждой\n"
    "\"3х5\" - 3 строки с пятью измерениями в каждой\n"
    "\"6x1\" - 6 строк по одному измерению в каждой\n"
    "\"6х2\" - 6 строк по два измерения в каждой"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n"
    ,
    "1",   "1",
    "2",   "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2",
    MEAS_NUM, IsActiveChoiceMeasuresNumber, nullptr, nullptr
)

DEF_CHOICE_3(mcMeasuresChannels, PageMeasures::self,
    "Каналы", "Channels",
    "По каким каналам выводить измерения",
    "Which channels to output measurement",
    "1",       "1",
    "2",       "2",
    "1 и 2",   "1 and 2",
    MEAS_SOURCE, IsActiveChoiceMeasuresChannels, nullptr, nullptr
)

DEF_CHOICE_2(mcMeasuresIsShow, PageMeasures::self,
    "Показывать", "Show",
    "Выводить или не выводить измерения на экран",
    "Output or output measurements on screen",
    "Нет", "No",
    "Да",  "Yes",
    SHOW_MEASURES, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcMeasuresSignal, PageMeasures::self,
    "Вид", "View",
    "Уменьшать или нет зону вывода сигнала для исключения перекрытия его результами измерений",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "Как есть",    "As is",
    "Уменьшать",   "Reduce",
    MODE_VIEW_SIGNALS, nullptr, nullptr, nullptr
)

static void PressSB_MeasTune_Exit()
{
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(sbExitMeasTune, PageMeasures::PageTune::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_MeasTune_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_6(pageTune, PageMeasures::PageTune::self, NamePage::SB_MeasTuneMeas,
    "НАСТРОИТЬ", "CONFIGURE",
    "Переход в режми точной настройки количества и видов измерений",
    "Transition to rezhm of exact control of quantity and types of measurements",
    sbExitMeasTune,
    Item::empty,
    Item::empty,
    Item::empty,
    sbMeasTuneMarkers,
    sbMeasTuneSettings,
    IsActiveButtonMeasuresTune, nullptr, nullptr, Measure::RotateRegSet
)

DEF_PAGE_5(pageMeasures, PageMain::self, NamePage::Measures,
    "ИЗМЕРЕНИЯ", "MEASURES",
    "Автоматические измерения",
    "Automatic measurements",
    mcMeasuresIsShow,
    mcMeasuresNumber,
    mcMeasuresChannels,
    mcMeasuresSignal,
    *PageMeasures::PageTune::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageMeasures::self = &pageMeasures;
const Page *PageMeasures::PageTune::self = &pageTune;
