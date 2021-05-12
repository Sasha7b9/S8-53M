// 2021/05/12 17:22:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cmath>


#define MAX_POS_U   200
#define MAX_POS_T   280


static void PressSB_Exit()
{
    Display::RemoveAddDrawFunction();
}


DEF_SMALL_BUTTON(sbExit, PageCursors::PageSet::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void SetSource(const Channel &ch)
{
    set.cursors.source = ch.value;
}


static void PressSB_Source()
{
    SetSource(set.cursors.source == ChA ? ChB : ChA);
}


static void DrawSB_SourceA(int x, int y)
{
    Text("1").Draw(x + 7, y + 5);
}


static void DrawSB_SourceB(int x, int y)
{
    Text("2").Draw(x + 7, y + 5);
}


static void DrawSB_Source(int x, int y)
{
    (set.cursors.source == ChA) ? DrawSB_SourceA(x, y) : DrawSB_SourceB(x, y);
}


static const arrayHints hintsSetSource =
{
    { DrawSB_SourceA, "канал 1", "channel 1" }, { DrawSB_SourceB, "канал 2", "channel 2" }
};


DEF_SMALL_BUTTON(sbSource, PageCursors::PageSet::self,
    "Источник", "Source",
    "Выбор канала для курсорных измерений",
    "Channel choice for measurements",
    nullptr, PressSB_Source, DrawSB_Source, &hintsSetSource
)


static void IncCursCntrlU(const Channel &ch)
{
    Math::CircleIncrease<int8>((int8 *)&set.cursors.cntrlU[ch], 0, 3);
}


static void PressSB_U()
{
    if (set.cursors.active.IsU() || set.cursors.cntrlU[set.cursors.source].IsDisable())
    {
        IncCursCntrlU(set.cursors.source);
    }

    set.cursors.active = CursActive::U;
}


static void DrawSB_U_Disable(int x, int y)
{
    Text("U").Draw(x + 7, y + 5);
}


static void DrawSB_U_Both_Disable(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, false, false);
}


static void DrawSB_U_Both_Enable(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, true, true);
}


static void DrawSB_U_Upper(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, true, false);
}


static void DrawSB_U_Lower(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, false, true);
}


static void DrawSB_Cursors_U(int x, int y)
{
    CursCntrl &cursCntrl = set.cursors.cntrlU[set.cursors.source];

    if (cursCntrl.IsDisable())
    {
        DrawSB_U_Disable(x, y);
    }
    else
    {
        if (!set.cursors.active.IsU())
        {
            DrawSB_U_Both_Disable(x, y);
        }
        else
        {
            Channel &source = set.cursors.source;
            bool condTop = false, condDown = false;

            CalculateConditions((int16)(Cursors::GetPosU(source, 0)),
                (int16)(Cursors::GetPosU(source, 1)), cursCntrl.value, &condTop, &condDown);

            if (condTop && condDown)
            {
                DrawSB_U_Both_Enable(x, y);
            }
            else if (condTop)
            {
                DrawSB_U_Upper(x, y);
            }
            else
            {
                DrawSB_U_Lower(x, y);
            }
        }
    }
}


static const arrayHints hintsSetU =
{
    { DrawSB_U_Disable,     "курсоры напряжения выключены",
                                    "cursors of tension are switched off" },
    { DrawSB_U_Both_Disable,"курсоры напряжения включены",
                                    "cursors of tension are switched on" },
    { DrawSB_U_Upper,       "курсоры напряжения включены, управление верхним курсором",
                                    "cursors of tension are switched on, control of the top cursor" },
    { DrawSB_U_Lower,       "курсоры напряжения включены, управление нижним курсором",
                                    "cursors of tension are switched on, control of the lower cursor" },
    { DrawSB_U_Both_Enable, "курсоры напряжения включены, управление обоими курсорами",
                                    "cursors of tension are switched on, control of both cursors" }
};


DEF_SMALL_BUTTON(sbtU, PageCursors::PageSet::self,
    "Курсоры U", "Cursors U",
    "Выбор курсоров напряжения для индикации и управления",
    "Choice of cursors of voltage for indication and management",
    nullptr, PressSB_U, DrawSB_Cursors_U, &hintsSetU
)


void IncCursCntrlT(const Channel &ch)
{
    Math::CircleIncrease<int8>((int8 *)&set.cursors.cntrlT[ch], 0, 3);
}


static void PressSB_Cursors_T()
{
    if (set.cursors.active.IsT() || set.cursors.cntrlT[set.cursors.source].IsDisable())
    {
        IncCursCntrlT(set.cursors.source);
    }

    set.cursors.active = CursActive::T;
}


static void DrawSB_Cursors_T_Disable(int x, int y)
{
    Text("T").Draw(x + 7, y + 5);
}


static void DrawSB_Cursors_T_Both_Disable(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, false, false);
}


static void DrawSB_Cursors_T_Both_Enable(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, true, true);
}


static void DrawSB_Cursors_T_Left(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, true, false);
}


static void DrawSB_Cursors_T_Right(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, false, true);
}


static void DrawSB_Cursors_T(int x, int y)
{
    Channel &source = set.cursors.source;
    CursCntrl &curs_cntrl = set.cursors.cntrlT[source];

    if (curs_cntrl.IsDisable())
    {
        DrawSB_Cursors_T_Disable(x, y);
    }
    else
    {
        if (!set.cursors.active.IsT())
        {
            DrawSB_Cursors_T_Both_Disable(x, y);
        }
        else
        {
            bool cond_left = false;
            bool cond_down = false;

            CalculateConditions((int16)(set.cursors.posT[source][0]),
                (int16)(set.cursors.posT[source][1]), curs_cntrl.value, &cond_left, &cond_down);

            if (cond_left && cond_down)
            {
                DrawSB_Cursors_T_Both_Enable(x, y);
            }
            else if (cond_left)
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


static const arrayHints hintsSetT =
{
    { DrawSB_Cursors_T_Disable,         "курсоры времени выключены",
                                        "cursors of time are switched off" },
    { DrawSB_Cursors_T_Both_Disable,    "курсоры времени включены",
                                        "cursors of time are switched on" },
    { DrawSB_Cursors_T_Left,            "курсоры времени включены, управление левым курсором",
                                        "cursors of time are switched on, control of the left cursor" },
    { DrawSB_Cursors_T_Right,           "курсоры времени включены, управление правым курсором",
                                        "cursors of time are switched on, control of the right cursor" },
    { DrawSB_Cursors_T_Both_Enable,     "курсоры времени включены, управление обоими курсорами",
                                        "cursors of time are switched on, control of both cursors" }
};


DEF_SMALL_BUTTON(sbT, PageCursors::PageSet::self,
    "Курсоры T", "Cursors T",
    "Выбор курсоров времени для индикации и управления",
    "Choice of cursors of time for indication and management",
    nullptr, PressSB_Cursors_T, DrawSB_Cursors_T, &hintsSetT
)


static void SetCursPos100(const Channel &ch)
{
    set.cursors.dU_100percents[ch] = std::fabsf(set.cursors.posU[ch][0] - set.cursors.posU[ch][1]);
    set.cursors.dT_100percents[ch] = std::fabsf(set.cursors.posT[ch][0] - set.cursors.posT[ch][1]);
}


static void PressSB_Cursors_100()
{
    SetCursPos100(set.cursors.source);
}


static void DrawSB_Cursors_100(int x, int y)
{
    Font::Set(TypeFont::S5);
    Text("100%").Draw(x + 2, y + 3);
    Font::Set(TypeFont::S8);
}


DEF_SMALL_BUTTON(sb100, PageCursors::PageSet::self,
    "100%", "100%",
    "Используется для процентных измерений. Нажатие помечает расстояние между активными курсорами как 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    nullptr, PressSB_Cursors_100, DrawSB_Cursors_100, nullptr
)


static void PressSB_Cursors_PointsPercents()
{
    Math::CircleIncrease<int8>((int8 *)&set.cursors.movement, 0, 1);
}


static void DrawSB_Cursors_PointsPercents_Percents(int x, int y)
{
    Text("\x83").Draw(x + 6, y + 5);
}


static void DrawSB_Cursors_PointsPercents_Points(int x, int y)
{
    Font::Set(TypeFont::S5);
    Text("тчк").Draw(x + 4, y + 3);
    Font::Set(TypeFont::S8);
}


static void DrawSB_Cursors_PointsPercents(int x, int y)
{
    if (set.cursors.movement.IsPercents())
    {
        DrawSB_Cursors_PointsPercents_Percents(x, y);
    }
    else
    {
        DrawSB_Cursors_PointsPercents_Points(x, y);
    }
}


static const arrayHints hintsSetPointsPercents =
{
    { DrawSB_Cursors_PointsPercents_Percents,   "шаг перемещения курсоров кратен одному проценту",
                                                "the step of movement of cursors is multiple to one percent" },
    { DrawSB_Cursors_PointsPercents_Points,     "шаг перемещения курсора кратен одному пикселю",
                                                "the step of movement of the cursor is multiple to one pixel" }
};


DEF_SMALL_BUTTON(sbPointsPercents, PageCursors::PageSet::self,
    "Перемещение", "Movement",
    "Выбор шага перемещения курсоров - проценты или точки",
    "Choice of a step of movement of cursors - percent or points",
    nullptr, PressSB_Cursors_PointsPercents, DrawSB_Cursors_PointsPercents, &hintsSetPointsPercents
)


static void SetShiftCursPosU(const Channel &ch, int numCur, float delta)
{
    set.cursors.posU[ch][numCur] = Math::Limitation<float>(set.cursors.posU[ch][numCur] - delta, 0, MAX_POS_U);
}


static void MoveCursUonPercentsOrPoints(int delta)
{
    CursCntrl &curs_cntrl = set.cursors.cntrlU[set.cursors.source];

    float value = (float)delta;

    Channel &source = set.cursors.source;

    if (set.cursors.movement.IsPercents())
    {
        value *= set.cursors.dU_100percents[source] / 100.0F;
    }

    if (curs_cntrl.Is1() || curs_cntrl.Is1_2())
    {
        SetShiftCursPosU(source, 0, value);
    }

    if (curs_cntrl.Is2() || curs_cntrl.Is1_2())
    {
        SetShiftCursPosU(source, 1, value);
    }

    Cursors_Update();
}


static void SetShiftCursPosT(const Channel &ch, int num_cur, float delta)
{
    set.cursors.posT[ch][num_cur] = Math::Limitation<float>(set.cursors.posT[ch][num_cur] + delta, 0, MAX_POS_T);
}


static void MoveCursTonPercentsOrPoints(int delta)
{
    Channel &source = set.cursors.source;
    CursCntrl &curs_cntrl = set.cursors.cntrlT[source];

    float value = (float)(delta);

    if (set.cursors.movement.IsPercents())
    {
        value *= set.cursors.dT_100percents[source] / 100.0F;
    }

    if (curs_cntrl.Is1() || curs_cntrl.Is1_2())
    {
        SetShiftCursPosT(source, 0, value);
    }

    if (curs_cntrl.Is2() || curs_cntrl.Is1_2())
    {
        SetShiftCursPosT(source, 1, value);
    }

    Cursors_Update();
}


static void OnRotate_RegSet_Set(int angle)
{
    if (set.cursors.active.IsU())
    {
        MoveCursUonPercentsOrPoints(angle);
    }
    else
    {
        MoveCursTonPercentsOrPoints(angle);
    }
    Sound::RegulatorShiftRotate();
}


DEF_PAGE_6(pageSet, PageCursors::self, NamePage::SB_Curs,
    "УСТАНОВИТЬ", "SET",
    "Переход в режим курсорных измерений",
    "Switch to cursor measures",
    sbExit,
    sbSource,
    sbtU,
    sbT,
    sb100,
    sbPointsPercents,
    nullptr, nullptr, nullptr, OnRotate_RegSet_Set
)

const Page *PageCursors::PageSet::self = &pageSet;
