#pragma once
#include "common/Utils/String_.h"


// Ќастройки курсорных измерений.
struct SettingsCursors
{
    CursCntrl       cntrlU[NumChannels];    // јктивные курсоры напр€жени€.
    CursCntrl       cntrlT[NumChannels];    // јктивные курсоры напр€жени€.
    Channel::E      source;                 // »сточник - к какому каналу относ€тс€ курсоры.
    float           posU[NumChannels][2];   // “екущие позиции курсоров напр€жени€ обоих каналов.
    float           posT[NumChannels][2];   // “екущие позиции курсоров времени обоих каналов.
    float           dU_100percents[2];      // –ассто€ние между курсорами напр€жени€ дл€ 100%, дл€ обоих каналов.
    float           dT_100percents[2];      // –ассто€ние между курсорами времени дл€ 100%, дл€ обоих каналов.
    CursMovement    movement;               //  ак перемещатьс€ курсорам - по точкам или по процентам.
    CursActive      active;                 //  акие курсоры сейчас активны.
    CursLookMode    look_mode[2];           // –ежимы слежени€ за курсорами дл€ двух пар курсоров.
    bool            show_freq;              // ”становленное в true значение, что нужно показывать на экране значение 1/dT между курсорами.
    bool            show;                   // ѕоказывать ли курсоры.
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
