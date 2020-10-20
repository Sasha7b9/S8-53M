#include "defines.h"
#include "common/Utils/Math_c.h"
#include "common/Display/Font/Font_c.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/PageMeasures.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Measures.h"

extern const Page pMeasures;

static CursCntrl::E GetMeasuresCursCntrlActive();       // Каким курсором из активной пары сейчас происходит управление.
bool PageMeasures::choiceMeasuresIsActive = false;

void DrawSB_MeasTune_Settings(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    Font::Set(TypeFont::_8);
}

void PressSB_MeasTune_Settings()
{
    Measure::ShorPressOnSmallButtonSettings();
}

void DrawSB_MeasTune_Markers(int x, int y)
{
    Font::Set(TypeFont::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    Font::Set(TypeFont::_8);
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
            LIMITATION(MEAS_POS_CUR_T0, static_cast<int16>(MEAS_POS_CUR_T0 - sign), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 1));
        }
        else
        {
            LIMITATION(MEAS_POS_CUR_U0, static_cast<int16>(MEAS_POS_CUR_U0 + sign), 0, 200);
        }
    }
    if (cursCntrl == CursCntrl::_2 || cursCntrl == CursCntrl::_1_2)
    {
        if (cursActive == CursActive::T)
        {
            LIMITATION(MEAS_POS_CUR_T1, static_cast<int16>(MEAS_POS_CUR_T1 - sign), 0, static_cast<int16>(sMemory_GetNumPoints(false) - 1));
        }
        else
        {
            LIMITATION(MEAS_POS_CUR_U1, static_cast<int16>(MEAS_POS_CUR_U1 + sign), 0, 200);
        }
    }
}

extern const Page mspMeasTune;

DEF_SMALL_BUTTON( sbMeasTuneSettings, &mspMeasTune,
    "Настройка", "Setup",
    "Позволяет выбрать необходимые измерения",
    "Allows to choose necessary measurements",
    nullptr, PressSB_MeasTune_Settings, DrawSB_MeasTune_Settings, nullptr
)

DEF_SMALL_BUTTON( sbMeasTuneMarkers, &mspMeasTune,                 // Включение / отключение маркера для режима измерений.
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

// ИЗМЕРЕНИЯ
extern const Page pMeasures;

// ИЗМЕРЕНИЯ -> Количество
DEF_CHOICE_7( mcMeasuresNumber, pMeasures,
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


// ИЗМЕРЕНИЯ -> Каналы
DEF_CHOICE_3(mcMeasuresChannels, &pMeasures,
    "Каналы", "Channels",
    "По каким каналам выводить измерения",
    "Which channels to output measurement",
    "1",       "1",
    "2",       "2",
    "1 и 2",   "1 and 2",
    MEAS_SOURCE, IsActiveChoiceMeasuresChannels, nullptr, nullptr
)


// ИЗМЕРЕНИЯ -> Показывать
DEF_CHOICE_2(mcMeasuresIsShow, &pMeasures,
    "Показывать", "Show",
    "Выводить или не выводить измерения на экран",
    "Output or output measurements on screen",
    "Нет", "No",
    "Да",  "Yes",
    SHOW_MEASURES, nullptr, nullptr, nullptr
)


// ИЗМЕРЕНИЯ -> Вид
DEF_CHOICE_2(mcMeasuresSignal, &pMeasures,
    "Вид", "View",
    "Уменьшать или нет зону вывода сигнала для исключения перекрытия его результами измерений",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "Как есть",    "As is",
    "Уменьшать",   "Reduce",
    MODE_VIEW_SIGNALS, nullptr, nullptr, nullptr
)


/**********************************************************************************************************************************************************/
// ИЗМЕРЕНИЯ -> ЗОНА
extern const Page mspMeasuresField;

// ИЗМЕРЕНИЯ -> ЗОНА -> Область
/*
const Choice mcMeasuresFieldType =
{
    TypeItem::Choice, &mspMeasuresField, 0,
    {
        "Область", "Field",
        "Выбор области, из которой будут браться значения для автоматических измерений",
        "Select an area from which the values will be taken for automatic measurements"
    },
    {   
        {"Экран",   "Screen"},
        {"Память",  "Memorye"},
        {"Курсоры", "Cursors"}
    },
    (int8*)&MEAS_FIELD
};
*/

CursCntrl::E GetMeasuresCursCntrlActive()
{
    if(MEAS_CURS_ACTIVE_IS_T)
    {
        return MEAS_CURS_CNTRL_T;
    }
    return MEAS_CURS_CNTRL_U;
}

static void PressSB_MeasTune_Exit()
{
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(sbExitMeasTune, &mspMeasTune,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_MeasTune_Exit, DrawSB_Exit, nullptr
)


// ИЗМЕРЕНИЯ - Настроить ////
DEF_PAGE_6(mspMeasTune, &pMeasures, NamePage::SB_MeasTuneMeas,
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


// ИЗМЕРЕНИЯ //////////////////////////
DEF_PAGE_5(pMeasures, PageMain::self, NamePage::Measures,
    "ИЗМЕРЕНИЯ", "MEASURES",
    "Автоматические измерения",
    "Automatic measurements",
    mcMeasuresIsShow,
    mcMeasuresNumber,
    mcMeasuresChannels,
    mcMeasuresSignal,
    mspMeasTune,
    nullptr, nullptr, nullptr, nullptr
)

void *PageMeasures::Tune::pointer = (void *)&mspMeasTune;
