#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "common/Display/Font/Font_c.h"
#include "common/Utils/Math_c.h"
#include "Hardware/Sound.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/ProcessingSignal.h"
#include <cmath>


using namespace Primitives;


static void MoveCursUonPercentsOrPoints(int delta);
static void MoveCursTonPercentsOrPoints(int delta);
static void SetShiftCursPosU(Channel::E chan, int numCur, float delta);    // �������� �������� ������� ������� ���������� �� delta �����
static void SetShiftCursPosT(Channel::E chan, int numCurs, float delta);   // �������� �������� ������� ������� ������� �� delta �����
static void SetCursPos100(Channel::E chan);                                // ��������� ������� ��������, ��������������� 100%.
static void IncCursCntrlU(Channel::E chan);                                // ������� ��������� ������.
static void IncCursCntrlT(Channel::E chan);                                // ������� ��������� ������.
static void SetCursPosU(Channel::E chan, int numCur, float pos);           // ���������� ������� ������� ����������.
static void SetCursPosT(Channel::E chan, int numCur, float pos);           // ���������� �������� ������� �� �������.


#define MAX_POS_U   200
#define MAX_POS_T   280


void SetCursSource(Channel::E chan)
{
    CURS_SOURCE = chan;
}



void IncCursCntrlU(Channel::E chan)
{
    CircleIncreaseInt8((int8*)&CURsU_CNTRL_CH(chan), 0, 3);
}



void IncCursCntrlT(Channel::E chan)
{
    CircleIncreaseInt8((int8*)&CURS_CNTRL_T(chan), 0, 3);
}



void Cursors_Update()
{
    Channel::E source = CURS_SOURCE;
    CursLookMode::E lookMode0 = CURS_LOOKMODE_0;
    CursLookMode::E lookMode1 = CURS_LOOKMODE_1;

    if((lookMode0 == CursLookMode::Voltage || lookMode0 == CursLookMode::Both) && CURS_ACTIVE_IS_T)
    {
        float posU0 = Processing::GetCursU(source, CURS_POS_T0(source));
        SetCursPosU(source, 0, posU0);
    }
    if((lookMode1 == CursLookMode::Voltage || lookMode1 == CursLookMode::Both)  && CURS_ACTIVE_IS_T)
    {
        float posU1 = Processing::GetCursU(source, CURS_POS_T1(source));
        SetCursPosU(source, 1, posU1);
    }
    if((lookMode0 == CursLookMode::Time || lookMode0 == CursLookMode::Both) && CURS_ACTIVE_IS_U)
    {
        float posU0 = CURS_POS_U0(source);
        float posT0 = Processing::GetCursT(source, posU0, 0);
        SetCursPosT(source, 0, posT0);
    }
    if((lookMode1 == CursLookMode::Time || lookMode1 == CursLookMode::Both) && CURS_ACTIVE_IS_U)
    {
        float posU1 = CURS_POS_U1(source);
        float posT1 = Processing::GetCursT(source, posU1, 1);
        SetCursPosT(source, 1, posT1);
    }
}


void SetCursPosU(Channel::E chan, int numCur, float pos)
{
    CURS_POS_U(chan, numCur) = LimitationFloat(pos, 0, MAX_POS_U);
}



void SetCursPosT(Channel::E chan, int numCur, float pos)
{
    CURS_POS_T(chan, numCur) = LimitationFloat(pos, 0, MAX_POS_T);
}


DEF_CHOICE_2(mcShow, PageCursors::self,
    "����������", "Shown",
    "��������/��������� �������.",
    "Enable/disable cursors.",
    "���", "No",
    "��", "Yes",
    CURS_SHOW, nullptr, nullptr, nullptr
)


DEF_CHOICE_4(mcTrackingT1U1, PageCursors::self,
    "�������� \x8e, \x9e", "Tracking \x8e, \x9e"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    "Sets the mode tracking for second cursor:\n"                   // WARN �������
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "����������", "Voltage",
    "�����", "Time",
    "������ � �����", "Volt and time",
    CURS_LOOKMODE_0, nullptr, nullptr, nullptr
)

DEF_CHOICE_4(mcTrackingT2U2, PageCursors::self,
    "�������� \x8f, \x9f", "Tracking \x8f, \x9f"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    "Sets the mode tracking cursors channel 2:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "����������", "Voltage",
    "�����", "Time",
    "������. � �����", "Volt. and time",
    CURS_LOOKMODE_1, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcShowFreq, PageCursors::self,
    "1/dT", "1/dT",
    "���� ������� \"���\", � ������ ������� ���� ��������� ��������, �������� ���������� ����� ��������� ������� - ������� �������, ���� ������ �������� ����� ���������� ����� ���������� ���������.",
    "If you select \"Enable\" in the upper right corner displays the inverse of the distance between cursors time - frequency signal, a period equal to the distance between the time cursors.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    CURSORS_SHOW_FREQ, nullptr, nullptr, nullptr
)

static void OnRotate_RegSet_Set(int angle)
{
    if (CURS_ACTIVE_IS_U)
    {
        MoveCursUonPercentsOrPoints(angle);
    }
    else
    {
        MoveCursTonPercentsOrPoints(angle);
    }
    Sound::RegulatorShiftRotate();
}

static void PressSB_Cursors_Exit(void)
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbSetExit, PageCursors::PageSet::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_Cursors_Exit, DrawSB_Exit, nullptr
)

static void DrawSB_Cursors_SourceA(int x, int y)
{
    Text("1").Draw(x + 7, y + 5);
}

static void DrawSB_Cursors_SourceB(int x, int y)
{
    Text("2").Draw(x + 7, y + 5);
}

static const arrayHints hintsSetSource =
{
    { DrawSB_Cursors_SourceA, "����� 1", "channel 1" }, { DrawSB_Cursors_SourceB, "����� 2", "channel 2" }
};

static void PressSB_Cursors_Source(void)
{
    Channel::E source = CURS_SOURCE_A ? Channel::B : Channel::A;
    SetCursSource(source);
}

static void DrawSB_Cursors_Source(int x, int y)
{
    CURS_SOURCE_A ? DrawSB_Cursors_SourceA(x, y) : DrawSB_Cursors_SourceB(x, y);
}


DEF_SMALL_BUTTON(sbSetSource, PageCursors::PageSet::self,
    "��������", "Source",
    "����� ������ ��� ��������� ���������",
    "Channel choice for measurements",
    nullptr, PressSB_Cursors_Source, DrawSB_Cursors_Source, &hintsSetSource
)

static void DrawSB_Cursors_U_Disable(int x, int y)
{
    Text("U").Draw(x + 7, y + 5);
}

static void DrawSB_Cursors_U_Both_Disable(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, false);
}

static void DrawSB_Cursors_U_Upper(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, false);
}

static void DrawSB_Cursors_U_Lower(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, true);
}

static void DrawSB_Cursors_U_Both_Enable(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, true);
}

static const arrayHints hintsSetU =
{
    { DrawSB_Cursors_U_Disable,     "������� ���������� ���������",
                                    "cursors of tension are switched off" },
    { DrawSB_Cursors_U_Both_Disable,"������� ���������� ��������",
                                    "cursors of tension are switched on" },
    { DrawSB_Cursors_U_Upper,       "������� ���������� ��������, ���������� ������� ��������",
                                    "cursors of tension are switched on, control of the top cursor" },
    { DrawSB_Cursors_U_Lower,       "������� ���������� ��������, ���������� ������ ��������",
                                    "cursors of tension are switched on, control of the lower cursor" },
    { DrawSB_Cursors_U_Both_Enable, "������� ���������� ��������, ���������� ������ ���������",
                                    "cursors of tension are switched on, control of both cursors" }
};

static void PressSB_Cursors_U(void)
{
    if (CURS_ACTIVE_IS_U || CURS_CNTRL_U_IS_DISABLE(CURS_SOURCE))
    {
        IncCursCntrlU(CURS_SOURCE);
    }
    CURS_ACTIVE = CursActive::U;
}

static void DrawSB_Cursors_U(int x, int y)
{
    CursCntrl::E cursCntrl = CURsU_CNTRL;
    if (cursCntrl == CursCntrl::Disable)
    {
        DrawSB_Cursors_U_Disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_IS_U)
        {
            DrawSB_Cursors_U_Both_Disable(x, y);
        }
        else
        {
            Channel::E source = CURS_SOURCE;
            bool condTop = false, condDown = false;
            CalculateConditions(static_cast<int16>(sCursors_GetCursPosU(source, 0)), static_cast<int16>(sCursors_GetCursPosU(source, 1)), cursCntrl, &condTop, &condDown);
            if (condTop && condDown)
            {
                DrawSB_Cursors_U_Both_Enable(x, y);
            }
            else if (condTop)
            {
                DrawSB_Cursors_U_Upper(x, y);
            }
            else
            {
                DrawSB_Cursors_U_Lower(x, y);
            }
        }
    }
}


DEF_SMALL_BUTTON(sbSetU, PageCursors::PageSet::self,
    "������� U", "Cursors U",
    "����� �������� ���������� ��� ��������� � ����������",
    "Choice of cursors of voltage for indication and management",
    nullptr, PressSB_Cursors_U, DrawSB_Cursors_U, &hintsSetU
)

static void DrawSB_Cursors_T_Disable(int x, int y)
{
    Text("T").Draw(x + 7, y + 5);
}

static void DrawSB_Cursors_T_Both_Disable(int x, int y)
{
    DrawMenuCursTime(x, y, false, false);
}

static void DrawSB_Cursors_T_Left(int x, int y)
{
    DrawMenuCursTime(x, y, true, false);
}

static void DrawSB_Cursors_T_Right(int x, int y)
{
    DrawMenuCursTime(x, y, false, true);
}

static void DrawSB_Cursors_T_Both_Enable(int x, int y)
{
    DrawMenuCursTime(x, y, true, true);
}

static const arrayHints hintsSetT =
{
    { DrawSB_Cursors_T_Disable,         "������� ������� ���������",
                                        "cursors of time are switched off" },
    { DrawSB_Cursors_T_Both_Disable,    "������� ������� ��������",
                                        "cursors of time are switched on" },
    { DrawSB_Cursors_T_Left,            "������� ������� ��������, ���������� ����� ��������",
                                        "cursors of time are switched on, control of the left cursor" },
    { DrawSB_Cursors_T_Right,           "������� ������� ��������, ���������� ������ ��������",
                                        "cursors of time are switched on, control of the right cursor" },
    { DrawSB_Cursors_T_Both_Enable,     "������� ������� ��������, ���������� ������ ���������",
                                        "cursors of time are switched on, control of both cursors" }
};

static void PressSB_Cursors_T(void)
{
    if (CURS_ACTIVE_IS_T || CURS_CNTRL_T_IS_DISABLE(CURS_SOURCE))
    {
        IncCursCntrlT(CURS_SOURCE);
    }
    CURS_ACTIVE = CursActive::T;
}

static void DrawSB_Cursors_T(int x, int y)
{
    Channel::E source = CURS_SOURCE;
    CursCntrl::E cursCntrl = CURS_CNTRL_T(source);
    if (cursCntrl == CursCntrl::Disable)
    {
        DrawSB_Cursors_T_Disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_IS_T)
        {
            DrawSB_Cursors_T_Both_Disable(x, y);
        }
        else
        {
            bool condLeft = false, condDown = false;
            CalculateConditions(static_cast<int16>(CURS_POS_T0(source)), static_cast<int16>(CURS_POS_T1(source)), cursCntrl, &condLeft, &condDown);
            if (condLeft && condDown)
            {
                DrawSB_Cursors_T_Both_Enable(x, y);
            }
            else if (condLeft)
            {
                DrawSB_Cursors_T_Left(x, y);
            }
            else
            {
                DrawSB_Cursors_T_Right(x, y);
            }
        }
    }
}

DEF_SMALL_BUTTON(sbSetT, PageCursors::PageSet::self,
    "������� T", "Cursors T",
    "����� �������� ������� ��� ��������� � ����������",
    "Choice of cursors of time for indication and management",
    nullptr, PressSB_Cursors_T, DrawSB_Cursors_T, &hintsSetT
)

static void PressSB_Cursors_100(void)
{
    SetCursPos100(CURS_SOURCE);
}

static void SetCursPos100(Channel::E chan)
{
    DELTA_U100(chan) = std::fabsf(CURS_POS_U0(chan) - CURS_POS_U1(chan));
    DELTA_T100(chan) = std::fabsf(CURS_POS_T0(chan) - CURS_POS_T1(chan));
}

static void DrawSB_Cursors_100(int x, int y)
{
    Font::Set(TypeFont::_5);
    Text("100%").Draw(x + 2, y + 3);
    Font::Set(TypeFont::_8);
}

DEF_SMALL_BUTTON(sbSet100, PageCursors::PageSet::self,
    "100%", "100%",
    "������������ ��� ���������� ���������. ������� �������� ���������� ����� ��������� ��������� ��� 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    nullptr, PressSB_Cursors_100, DrawSB_Cursors_100, nullptr
)

static void DrawSB_Cursors_PointsPercents_Percents(int x, int y)
{
    Text("\x83").Draw(x + 6, y + 5);
}

static void DrawSB_Cursors_PointsPercents_Points(int x, int y)
{
    Font::Set(TypeFont::_5);
    Text("���").Draw(x + 4, y + 3);
    Font::Set(TypeFont::_8);
}

static const arrayHints hintsSetPointsPercents =
{
    { DrawSB_Cursors_PointsPercents_Percents,   "��� ����������� �������� ������ ������ ��������",
                                                "the step of movement of cursors is multiple to one percent" },
    { DrawSB_Cursors_PointsPercents_Points,     "��� ����������� ������� ������ ������ �������",
                                                "the step of movement of the cursor is multiple to one pixel" }
};

static void PressSB_Cursors_PointsPercents(void)
{
    CircleIncreaseInt8((int8 *)&CURS_MOVEMENT, 0, 1);
}

static void DrawSB_Cursors_PointsPercents(int x, int y)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        DrawSB_Cursors_PointsPercents_Percents(x, y);
    }
    else
    {
        DrawSB_Cursors_PointsPercents_Points(x, y);
    }
}

DEF_SMALL_BUTTON(sbSetPointsPercents, PageCursors::PageSet::self,
    "�����������", "Movement",
    "����� ���� ����������� �������� - �������� ��� �����",
    "Choice of a step of movement of cursors - percent or points",
    nullptr, PressSB_Cursors_PointsPercents, DrawSB_Cursors_PointsPercents, &hintsSetPointsPercents
)

DEF_PAGE_6(mspSet, PageCursors::self, NamePage::SB_Curs,
    "����������", "SET",
    "������� � ����� ��������� ���������",
    "Switch to cursor measures",
    sbSetExit,              // ������� - ���������� - �����
    sbSetSource,            // ������� - ���������� - ��������
    sbSetU,                 // ������� - ���������� - ������� U
    sbSetT,                 // ������� - ���������� - ������� T
    sbSet100,               // ������� - ���������� - 100%
    sbSetPointsPercents,    // ������� - ���������� - �����������
    nullptr, nullptr, nullptr, OnRotate_RegSet_Set
)

const Page *PageCursors::PageSet::self = &mspSet;


DEF_PAGE_5(pCursors, PageMain::self, NamePage::Cursors,
    "�������", "CURSORS",
    "��������� ���������.",
    "Cursor measurements.",
    mcShow,             // ������� - ����������
    mcTrackingT1U1,     // ������� - ������� T1,U1
    mcTrackingT2U2,     // ������� - ������� T2,U2
    mcShowFreq,         // ������� - 1/dT
    mspSet,              // ������� - ����������
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageCursors::self = &pCursors;


static void MoveCursUonPercentsOrPoints(int delta)
{
    CursCntrl::E cursCntrl = CURsU_CNTRL;

    float value = static_cast<float>(delta);

    Channel::E source = CURS_SOURCE;

    if(CURS_MOVEMENT_IS_PERCENTS)
    {
        value *= DELTA_U100(source) / 100.0F;
    }

    if(cursCntrl == CursCntrl::_1 || cursCntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosU(source, 0, value);
    }
    if(cursCntrl == CursCntrl::_2 || cursCntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosU(source, 1, value);
    }
    Cursors_Update();
}

static void SetShiftCursPosU(Channel::E chan, int numCur, float delta)
{
    CURS_POS_U(chan, numCur) = LimitationFloat(CURS_POS_U(chan, numCur) - delta, 0, MAX_POS_U);
}

static void MoveCursTonPercentsOrPoints(int delta)
{
    Channel::E source = CURS_SOURCE;
    CursCntrl::E cursCntrl = CURS_CNTRL_T(source);

    float value = static_cast<float>(delta);

    if(CURS_MOVEMENT_IS_PERCENTS)
    {
        value *= DELTA_T100(source) / 100.0F;
    }

    if(cursCntrl == CursCntrl::_1 || cursCntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosT(source, 0, value);
    }
    if(cursCntrl == CursCntrl::_2 || cursCntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosT(source, 1, value);
    }
    Cursors_Update();
}

static void SetShiftCursPosT(Channel::E chan, int numCur, float delta)
{
    CURS_POS_T(chan, numCur) = LimitationFloat(CURS_POS_T(chan, numCur) + delta, 0, MAX_POS_T);
}
