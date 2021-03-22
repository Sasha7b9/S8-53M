#include "defines.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include <cmath>


using namespace Primitives;


static void MoveCursUonPercentsOrPoints(int delta);
static void MoveCursTonPercentsOrPoints(int delta);
static void SetShiftCursPosU(Channel::E ch, int numCur, float delta);    // Изменить значение позиции курсора напряжения на delta точек
static void SetShiftCursPosT(Channel::E ch, int numCurs, float delta);   // Изменить значение позиции курсора времени на delta точек
static void SetCursPos100(Channel::E ch);                                // Запомнить позиции курсоров, соответствующие 100%.
static void IncCursCntrlU(Channel::E ch);                                // Выбрать следующий курсор.
static void IncCursCntrlT(Channel::E ch);                                // Выбрать следующий курсор.
static void SetCursPosU(Channel::E ch, int numCur, float pos);           // Установить позицию курсора напряжения.
static void SetCursPosT(Channel::E ch, int numCur, float pos);           // Установить значение курсора по времени.


#define MAX_POS_U   200
#define MAX_POS_T   280


void SetCursSource(const Channel &ch)
{
    set.cursors.source = ch.value;
}



void IncCursCntrlU(Channel::E ch)
{
    GF::CircleIncrease<int8>((int8*)&set.cursors.cntrlU[ch], 0, 3);
}



void IncCursCntrlT(Channel::E ch)
{
    GF::CircleIncrease<int8>((int8*)&set.cursors.cntrlT[ch], 0, 3);
}



void Cursors_Update()
{
    Channel::E source = set.cursors.source;
    CursLookMode &look_mode_0 = set.cursors.look_mode[0];
    CursLookMode &look_mode_1 = set.cursors.look_mode[1];

    if((look_mode_0.IsVoltage() || look_mode_0.IsTime()) && set.cursors.active.IsT())
    {
        float posU0 = Processing::GetCursU(source, set.cursors.posT[source][0]);
        SetCursPosU(source, 0, posU0);
    }
    if((look_mode_1.IsVoltage() || look_mode_1.IsBoth())  && set.cursors.active.IsT())
    {
        float posU1 = Processing::GetCursU(source, set.cursors.posT[source][1]);
        SetCursPosU(source, 1, posU1);
    }
    if((look_mode_0.IsTime() || look_mode_0.IsBoth()) && set.cursors.active.IsU())
    {
        float posU0 = set.cursors.posU[source][0];
        float posT0 = Processing::GetCursT(source, posU0, 0);
        SetCursPosT(source, 0, posT0);
    }
    if((look_mode_1.IsTime() || look_mode_1.IsBoth()) && set.cursors.active.IsU())
    {
        float posU1 = set.cursors.posU[source][1];
        float posT1 = Processing::GetCursT(source, posU1, 1);
        SetCursPosT(source, 1, posT1);
    }
}


void SetCursPosU(Channel::E ch, int num_cur, float pos)
{
    set.cursors.posU[ch][num_cur] = Math::Limitation(pos, 0.0f, (float)MAX_POS_U);
}



void SetCursPosT(Channel::E ch, int num_cur, float pos)
{
    set.cursors.posT[ch][num_cur] = Math::Limitation(pos, 0.0f, (float)MAX_POS_T);
}


DEF_CHOICE_2(mcShow, PageCursors::self,
    "Показывать", "Shown",
    "Включает/отключает курсоры.",
    "Enable/disable cursors.",
    "Нет", "No",
    "Да", "Yes",
    set.cursors.show, nullptr, nullptr, nullptr
)


DEF_CHOICE_4(mcTrackingT1U1, PageCursors::self,
    "Слежение \x8e, \x9e", "Tracking \x8e, \x9e"
    ,
    "Задаёт режим слежения за первым курсором времени и напряжения:\n"
    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают изменения сигнала.\n"
    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают изменения сигнала.\n"
    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор производилось последнее воздействие."
    ,
    "Sets the mode tracking for second cursor:\n"                   // WARN Перевод
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "Напряжение", "Voltage",
    "Время", "Time",
    "Напряж и время", "Volt and time",
    set.cursors.look_mode[0], nullptr, nullptr, nullptr
)

DEF_CHOICE_4(mcTrackingT2U2, PageCursors::self,
    "Слежение \x8f, \x9f", "Tracking \x8f, \x9f"
    ,
    "Задаёт режим слежения за вторым курсором времени и напряжения:\n"
    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают изменения сигнала.\n"
    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают изменения сигнала.\n"
    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор производилось последнее воздействие."
    ,
    "Sets the mode tracking cursors channel 2:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "Напряжение", "Voltage",
    "Время", "Time",
    "Напряж. и время", "Volt. and time",
    set.cursors.look_mode[1], nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcShowFreq, PageCursors::self,
    "1/dT", "1/dT",
    "Если выбрано \"Вкл\", в правом верхнем углу выводится величина, обратная расстоянию между курсорами времени - частота сигнала, один период которого равен расстоянию между временными курсорами.",
    "If you select \"Enable\" in the upper right corner displays the inverse of the distance between cursors time - frequency signal, a period equal to the distance between the time cursors.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.cursors.show_freq, nullptr, nullptr, nullptr
)

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

static void PressSB_Cursors_Exit()
{
    Display::RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(sbSetExit, PageCursors::PageSet::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
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
    { DrawSB_Cursors_SourceA, "канал 1", "channel 1" }, { DrawSB_Cursors_SourceB, "канал 2", "channel 2" }
};

static void PressSB_Cursors_Source()
{
    SetCursSource(set.cursors.source == ChA ? ChB : ChA);
}

static void DrawSB_Cursors_Source(int x, int y)
{
    (set.cursors.source == ChA) ? DrawSB_Cursors_SourceA(x, y) : DrawSB_Cursors_SourceB(x, y);
}


DEF_SMALL_BUTTON(sbSetSource, PageCursors::PageSet::self,
    "Источник", "Source",
    "Выбор канала для курсорных измерений",
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
    { DrawSB_Cursors_U_Disable,     "курсоры напряжения выключены",
                                    "cursors of tension are switched off" },
    { DrawSB_Cursors_U_Both_Disable,"курсоры напряжения включены",
                                    "cursors of tension are switched on" },
    { DrawSB_Cursors_U_Upper,       "курсоры напряжения включены, управление верхним курсором",
                                    "cursors of tension are switched on, control of the top cursor" },
    { DrawSB_Cursors_U_Lower,       "курсоры напряжения включены, управление нижним курсором",
                                    "cursors of tension are switched on, control of the lower cursor" },
    { DrawSB_Cursors_U_Both_Enable, "курсоры напряжения включены, управление обоими курсорами",
                                    "cursors of tension are switched on, control of both cursors" }
};

static void PressSB_Cursors_U()
{
    if (set.cursors.active.IsU() || set.cursors.cntrlU[set.cursors.source].IsDisable())
    {
        IncCursCntrlU(set.cursors.source);
    }

    set.cursors.active = CursActive::U;
}

static void DrawSB_Cursors_U(int x, int y)
{
    CursCntrl &cursCntrl = set.cursors.cntrlU[set.cursors.source];

    if (cursCntrl.IsDisable())
    {
        DrawSB_Cursors_U_Disable(x, y);
    }
    else
    {
        if (!set.cursors.active.IsU())
        {
            DrawSB_Cursors_U_Both_Disable(x, y);
        }
        else
        {
            Channel::E source = set.cursors.source;
            bool condTop = false, condDown = false;

            CalculateConditions((int16)(Cursors::GetPosU(source, 0)),
                (int16)(Cursors::GetPosU(source, 1)), cursCntrl.value, &condTop, &condDown);

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
    "Курсоры U", "Cursors U",
    "Выбор курсоров напряжения для индикации и управления",
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

static void PressSB_Cursors_T()
{
    if (set.cursors.active.IsT() || set.cursors.cntrlT[set.cursors.source].IsDisable())
    {
        IncCursCntrlT(set.cursors.source);
    }

    set.cursors.active = CursActive::T;
}

static void DrawSB_Cursors_T(int x, int y)
{
    Channel::E source = set.cursors.source;
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

DEF_SMALL_BUTTON(sbSetT, PageCursors::PageSet::self,
    "Курсоры T", "Cursors T",
    "Выбор курсоров времени для индикации и управления",
    "Choice of cursors of time for indication and management",
    nullptr, PressSB_Cursors_T, DrawSB_Cursors_T, &hintsSetT
)

static void PressSB_Cursors_100()
{
    SetCursPos100(set.cursors.source);
}

static void SetCursPos100(Channel::E ch)
{
    set.cursors.dU_100percents[ch] = std::fabsf(set.cursors.posU[ch][0] - set.cursors.posU[ch][1]);
    set.cursors.dT_100percents[ch] = std::fabsf(set.cursors.posT[ch][0] - set.cursors.posT[ch][1]);
}

static void DrawSB_Cursors_100(int x, int y)
{
    Font::Set(TypeFont::S5);
    Text("100%").Draw(x + 2, y + 3);
    Font::Set(TypeFont::S8);
}

DEF_SMALL_BUTTON(sbSet100, PageCursors::PageSet::self,
    "100%", "100%",
    "Используется для процентных измерений. Нажатие помечает расстояние между активными курсорами как 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    nullptr, PressSB_Cursors_100, DrawSB_Cursors_100, nullptr
)

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

static const arrayHints hintsSetPointsPercents =
{
    { DrawSB_Cursors_PointsPercents_Percents,   "шаг перемещения курсоров кратен одному проценту",
                                                "the step of movement of cursors is multiple to one percent" },
    { DrawSB_Cursors_PointsPercents_Points,     "шаг перемещения курсора кратен одному пикселю",
                                                "the step of movement of the cursor is multiple to one pixel" }
};

static void PressSB_Cursors_PointsPercents()
{
    GF::CircleIncrease<int8>((int8 *)&set.cursors.movement, 0, 1);
}

static void DrawSB_Cursors_PointsPercents(int x, int y)
{
    if (CursMovement::IsPercents())
    {
        DrawSB_Cursors_PointsPercents_Percents(x, y);
    }
    else
    {
        DrawSB_Cursors_PointsPercents_Points(x, y);
    }
}

DEF_SMALL_BUTTON(sbSetPointsPercents, PageCursors::PageSet::self,
    "Перемещение", "Movement",
    "Выбор шага перемещения курсоров - проценты или точки",
    "Choice of a step of movement of cursors - percent or points",
    nullptr, PressSB_Cursors_PointsPercents, DrawSB_Cursors_PointsPercents, &hintsSetPointsPercents
)

DEF_PAGE_6(pageSet, PageCursors::self, NamePage::SB_Curs,
    "УСТАНОВИТЬ", "SET",
    "Переход в режим курсорных измерений",
    "Switch to cursor measures",
    sbSetExit,              // КУРСОРЫ - УСТАНОВИТЬ - Выход
    sbSetSource,            // КУРСОРЫ - УСТАНОВИТЬ - Источник
    sbSetU,                 // КУРСОРЫ - УСТАНОВИТЬ - Курсоры U
    sbSetT,                 // КУРСОРЫ - УСТАНОВИТЬ - Курсоры T
    sbSet100,               // КУРСОРЫ - УСТАНОВИТЬ - 100%
    sbSetPointsPercents,    // КУРСОРЫ - УСТАНОВИТЬ - Перемещение
    nullptr, nullptr, nullptr, OnRotate_RegSet_Set
)

DEF_PAGE_5(pageCursors, PageMain::self, NamePage::Cursors,
    "КУРСОРЫ", "CURSORS",
    "Курсорные измерения.",
    "Cursor measurements.",
    mcShow,             // КУРСОРЫ - Показывать
    mcTrackingT1U1,     // КУРСОРЫ - Курсоры T1,U1
    mcTrackingT2U2,     // КУРСОРЫ - Курсоры T2,U2
    mcShowFreq,         // КУРОСРЫ - 1/dT
    pageSet,            // КУРСОРЫ - УСТАНОВИТЬ
    nullptr, nullptr, nullptr, nullptr
)

static void MoveCursUonPercentsOrPoints(int delta)
{
    CursCntrl &curs_сntrl = set.cursors.cntrlU[set.cursors.source];

    float value = (float)delta;

    Channel::E source = set.cursors.source;

    if(CursMovement::IsPercents())
    {
        value *= set.cursors.dU_100percents[source] / 100.0F;
    }

    if(curs_сntrl.Is1() || curs_сntrl.Is1_2())
    {
        SetShiftCursPosU(source, 0, value);
    }

    if(curs_сntrl.Is2() || curs_сntrl.Is1_2())
    {
        SetShiftCursPosU(source, 1, value);
    }

    Cursors_Update();
}

static void SetShiftCursPosU(Channel::E ch, int numCur, float delta)
{
    set.cursors.posU[ch][numCur] = Math::Limitation<float>(set.cursors.posU[ch][numCur] - delta, 0, MAX_POS_U);
}

static void MoveCursTonPercentsOrPoints(int delta)
{
    Channel::E source = set.cursors.source;
    CursCntrl &curs_cntrl = set.cursors.cntrlT[source];

    float value = static_cast<float>(delta);

    if(CursMovement::IsPercents())
    {
        value *= set.cursors.dT_100percents[source] / 100.0F;
    }

    if(curs_cntrl.Is1() || curs_cntrl.Is1_2())
    {
        SetShiftCursPosT(source, 0, value);
    }

    if(curs_cntrl.Is2() || curs_cntrl.Is1_2())
    {
        SetShiftCursPosT(source, 1, value);
    }

    Cursors_Update();
}

static void SetShiftCursPosT(Channel::E ch, int num_cur, float delta)
{
    set.cursors.posT[ch][num_cur] = Math::Limitation<float>(set.cursors.posT[ch][num_cur] + delta, 0, MAX_POS_T);
}

const Page *PageCursors::self = &pageCursors;
const Page *PageCursors::PageSet::self = &pageSet;
