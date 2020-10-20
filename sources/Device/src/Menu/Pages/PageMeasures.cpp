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

static CursCntrl::E GetMeasuresCursCntrlActive();       // ����� �������� �� �������� ���� ������ ���������� ����������.
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
    "���������", "Setup",
    "��������� ������� ����������� ���������",
    "Allows to choose necessary measurements",
    nullptr, PressSB_MeasTune_Settings, DrawSB_MeasTune_Settings, nullptr
)

DEF_SMALL_BUTTON( sbMeasTuneMarkers, &mspMeasTune,                 // ��������� / ���������� ������� ��� ������ ���������.
    "������", "Marker",
    "��������� ���������� ������� ��� ����������� �������� ���������",
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

// ���������
extern const Page pMeasures;

// ��������� -> ����������
DEF_CHOICE_7( mcMeasuresNumber, pMeasures,
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
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


// ��������� -> ������
DEF_CHOICE_3(mcMeasuresChannels, &pMeasures,
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1",       "1",
    "2",       "2",
    "1 � 2",   "1 and 2",
    MEAS_SOURCE, IsActiveChoiceMeasuresChannels, nullptr, nullptr
)


// ��������� -> ����������
DEF_CHOICE_2(mcMeasuresIsShow, &pMeasures,
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��",  "Yes",
    SHOW_MEASURES, nullptr, nullptr, nullptr
)


// ��������� -> ���
DEF_CHOICE_2(mcMeasuresSignal, &pMeasures,
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",    "As is",
    "���������",   "Reduce",
    MODE_VIEW_SIGNALS, nullptr, nullptr, nullptr
)


/**********************************************************************************************************************************************************/
// ��������� -> ����
extern const Page mspMeasuresField;

// ��������� -> ���� -> �������
/*
const Choice mcMeasuresFieldType =
{
    TypeItem::Choice, &mspMeasuresField, 0,
    {
        "�������", "Field",
        "����� �������, �� ������� ����� ������� �������� ��� �������������� ���������",
        "Select an area from which the values will be taken for automatic measurements"
    },
    {   
        {"�����",   "Screen"},
        {"������",  "Memorye"},
        {"�������", "Cursors"}
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
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_MeasTune_Exit, DrawSB_Exit, nullptr
)


// ��������� - ��������� ////
DEF_PAGE_6(mspMeasTune, &pMeasures, NamePage::SB_MeasTuneMeas,
    "���������", "CONFIGURE",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    "Transition to rezhm of exact control of quantity and types of measurements",
    sbExitMeasTune,
    Item::empty,
    Item::empty,
    Item::empty,
    sbMeasTuneMarkers,
    sbMeasTuneSettings,
    IsActiveButtonMeasuresTune, nullptr, nullptr, Measure::RotateRegSet
)


// ��������� //////////////////////////
DEF_PAGE_5(pMeasures, PageMain::self, NamePage::Measures,
    "���������", "MEASURES",
    "�������������� ���������",
    "Automatic measurements",
    mcMeasuresIsShow,
    mcMeasuresNumber,
    mcMeasuresChannels,
    mcMeasuresSignal,
    mspMeasTune,
    nullptr, nullptr, nullptr, nullptr
)

void *PageMeasures::Tune::pointer = (void *)&mspMeasTune;
