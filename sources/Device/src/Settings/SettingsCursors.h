#pragma once
#include "common/Utils/String_.h"


#define CURsU_CNTRL_CH(ch)          (set.cursors.cntrlU[ch])                    // SettingsCursors.cntrlU
#define CURsU_CNTRL                 (CURsU_CNTRL_CH(set.cursors.source))
#define CURS_CNTRL_U_IS_DISABLE(ch) (CURsU_CNTRL_CH(ch) == CursCntrl::Disable)

#define CURS_CNTRL_T(ch)            (set.cursors.cntrlT[ch])                    // SettingsCursors.cntrlT
#define CURS_CNTRL_T_IS_DISABLE(ch) (CURS_CNTRL_T(ch) == CursCntrl::Disable)

#define CURS_POS_U(ch, num)         (set.cursors.posCurU[ch][num])              // SettingsCursors.posCurU
#define CURS_POS_U0(ch)             (CURS_POS_U(ch, 0))
#define CURS_POS_U1(ch)             (CURS_POS_U(ch, 1))

#define CURS_POS_T(ch, num)         (set.cursors.posCurT[ch][num])              // SettingsCursors.posCurT
#define CURS_POS_T0(ch)             (CURS_POS_T(ch, 0))
#define CURS_POS_T1(ch)             (CURS_POS_T(ch, 1))

#define DELTA_U100(ch)              (set.cursors.deltaU100percents[ch])

#define DELTA_T100(ch)              (set.cursors.deltaT100percents[ch])

#define CURS_ACTIVE                 (set.cursors.active)                        // SettingsCursors.active
#define CURS_ACTIVE_IS_T            (CURS_ACTIVE == CursActive::T)
#define CURS_ACTIVE_IS_U            (CURS_ACTIVE == CursActive::U)

#define CURS_LOOKMODE(num)          (set.cursors.lookMode[num])                 // SettingsCursors.lookMode
#define CURS_LOOKMODE_0             (CURS_LOOKMODE(0))
#define CURS_LOOKMODE_1             (CURS_LOOKMODE(1))

#define CURS_SHOW                   (set.cursors.showCursors)                   // SettingsCursors.showCursors




// Ќастройки курсорных измерений.
struct SettingsCursors
{
    CursCntrl::E    cntrlU[NumChannels];        // јктивные курсоры напр€жени€.
    CursCntrl::E    cntrlT[NumChannels];        // јктивные курсоры напр€жени€.
    Channel::E      source;                     // »сточник - к какому каналу относ€тс€ курсоры.
    float           posCurU[NumChannels][2];    // “екущие позиции курсоров напр€жени€ обоих каналов.
    float           posCurT[NumChannels][2];    // “екущие позиции курсоров времени обоих каналов.
    float           deltaU100percents[2];       // –ассто€ние между курсорами напр€жени€ дл€ 100%, дл€ обоих каналов.
    float           deltaT100percents[2];       // –ассто€ние между курсорами времени дл€ 100%, дл€ обоих каналов.
    CursMovement::E movement;                   //  ак перемещатьс€ курсорам - по точкам или по процентам.
    CursActive::E   active;                     //  акие курсоры сейчас активны.
    CursLookMode::E lookMode[2];                // –ежимы слежени€ за курсорами дл€ двух пар курсоров.
    bool            show_freq;                  // ”становленное в true значение, что нужно показывать на экране значение 1/dT между курсорами.
    bool            showCursors;                // ѕоказывать ли курсоры.
};


struct Cursors
{
    // ѕолучить позицию курсора напр€жени€.
    static float GetPosU(const Channel &ch, int num);

    // ¬озвращает true,если нужно рисовать курсоры.
    static bool NecessaryDraw();

    // ѕолучить строку курсора напр€жени€.
    static String GetVoltage(const Channel &source, int numCur);

    // ѕолучить строку курсора времени.
    static String GetTime(const Channel &source, int numCur);

    // ѕолучить строку процентов курсоров напр€жени€.
    static String GetPercentsU(const Channel &source);

    // ѕолучить строку процентов курсоров времени.
    static String GetPercentsT(const Channel &source);

    // Ќарисовать курсоры курсорных измерений.
    static void Draw();

private:

    // Ќарисовать вертикальный курсор курсорных измерений.
    // x         - числовое значение курсора
    // y_tearing - координата места, в котором необходимо сделать разрыв дл€ квадрата пересечени€
    static void DrawVertical(int x, int yTearing);

    // Ќарисовать горизонтальный курсор курсорных измерений
    // y         - числовое значение курсора
    // x_tearing - координата места, в котором необходимо сделать разрыв дл€ квадрата пересечени€
    static void DrawHorizontal(int y, int x_tearing);
};
