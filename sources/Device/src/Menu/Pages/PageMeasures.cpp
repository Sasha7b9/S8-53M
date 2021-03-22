#include "defines.h"
#include "common/Utils/Math_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/Measures.h"

bool PageMeasures::choiceMeasuresIsActive = false;

// Каким курсором в данный момент происходит управление
static CursCntrl::E GetMeasuresCursCntrlActive()
{
    return set.measures.curs_active.IsT() ? set.measures.cntrlT.value : set.measures.cntrlU.value;
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
    CursCntrl::E curs_cntrl = GetMeasuresCursCntrlActive();
    CursActive &curs_active = set.measures.curs_active;

    int sign = Math::Sign(angle);

    if (curs_cntrl == CursCntrl::_1 || curs_cntrl == CursCntrl::_1_2)
    {
        if (curs_active.IsT())
        {
            LIMITATION(set.measures.posT[0], static_cast<int16>(SettingsMeasures::PosCursorT(0) - sign), 0,
                static_cast<int16>(SettingsMemory::GetNumPoints(false) - 1));
        }
        else
        {
            LIMITATION(set.measures.posU[0], static_cast<int16>(SettingsMeasures::PosCursorU(0) + sign), 0, 200);
        }
    }
    if (curs_cntrl == CursCntrl::_2 || curs_cntrl == CursCntrl::_1_2)
    {
        if (curs_active.IsT())
        {
            LIMITATION(set.measures.posT[1], static_cast<int16>(SettingsMeasures::PosCursorT(1) - sign), 0,
                static_cast<int16>(SettingsMemory::GetNumPoints(false) - 1));
        }
        else
        {
            LIMITATION(set.measures.posU[1], static_cast<int16>(SettingsMeasures::PosCursorU(1) + sign), 0, 200);
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
    return SettingsMeasures::ShowMeasures();
}

bool IsActiveChoiceMeasuresChannels()
{
    return SettingsMeasures::ShowMeasures();
}

bool IsActivePageMeasuresFields()
{
    return SettingsMeasures::ShowMeasures();
}

bool IsActiveChoiceMeasuresSignal()
{
    return SettingsMeasures::ShowMeasures();
}

bool IsActiveButtonMeasuresTune()
{
    return SettingsMeasures::ShowMeasures();
}

bool IsActiveButtonMeasuresFieldSet()
{
    return set.measures.field.IsHand();
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
    set.measures.number, IsActiveChoiceMeasuresNumber, nullptr, nullptr
)

DEF_CHOICE_3(mcMeasuresChannels, PageMeasures::self,
    "Каналы", "Channels",
    "По каким каналам выводить измерения",
    "Which channels to output measurement",
    "1",       "1",
    "2",       "2",
    "1 и 2",   "1 and 2",
    set.measures.source, IsActiveChoiceMeasuresChannels, nullptr, nullptr
)

DEF_CHOICE_2(mcMeasuresIsShow, PageMeasures::self,
    "Показывать", "Show",
    "Выводить или не выводить измерения на экран",
    "Output or output measurements on screen",
    "Нет", "No",
    "Да",  "Yes",
    set.measures.show, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcMeasuresSignal, PageMeasures::self,
    "Вид", "View",
    "Уменьшать или нет зону вывода сигнала для исключения перекрытия его результами измерений",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "Как есть",    "As is",
    "Уменьшать",   "Reduce",
    set.measures.mode_view_signals, nullptr, nullptr, nullptr
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
