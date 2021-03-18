#pragma once
#include "common/Utils/String_.h"


#define CURSORS_SHOW_FREQ           (set.cursors.showFreq)                      // SettingsCursors.showFreq
#define CURS_MOVEMENT               (set.cursors.movement)                      // SettingsCursors.movement
#define CURS_MOVEMENT_IS_PERCENTS   (CURS_MOVEMENT == CursMovement::Percents)

#define CURS_SOURCE                 (set.cursors.source)                        // SettingsCursors.source
#define CURS_SOURCE_A               (CURS_SOURCE == ChA)

#define CURsU_CNTRL_CH(ch)          (set.cursors.cntrlU[ch])                    // SettingsCursors.cntrlU
#define CURsU_CNTRL                 (CURsU_CNTRL_CH(CURS_SOURCE))
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




// ��������� ��������� ���������.
struct SettingsCursors
{
    CursCntrl::E    cntrlU[NumChannels];        // �������� ������� ����������.
    CursCntrl::E    cntrlT[NumChannels];        // �������� ������� ����������.
    Channel::E      source;                     // �������� - � ������ ������ ��������� �������.
    float           posCurU[NumChannels][2];    // ������� ������� �������� ���������� ����� �������.
    float           posCurT[NumChannels][2];    // ������� ������� �������� ������� ����� �������.
    float           deltaU100percents[2];       // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float           deltaT100percents[2];       // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    CursMovement::E movement;                   // ��� ������������ �������� - �� ������ ��� �� ���������.
    CursActive::E   active;                     // ����� ������� ������ �������.
    CursLookMode::E lookMode[2];                // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool            showFreq;                   // ������������� � true ��������, ��� ����� ���������� �� ������ �������� 1/dT ����� ���������.
    bool            showCursors;                // ���������� �� �������.
};


struct Cursors
{
    // �������� ������� ������� ����������.
    static float GetCursPosU(const Channel &ch, int num);

    // ���������� true,���� ����� �������� �������.
    static bool NecessaryDrawCursors();

    // �������� ������ ������� ����������.
    static String GetCursVoltage(const Channel &source, int numCur);

    // �������� ������ ������� �������.
    static String GetCursorTime(const Channel &source, int numCur);

    // �������� ������ ��������� �������� ����������.
    static String GetCursorPercentsU(const Channel &source);

    // �������� ������ ��������� �������� �������.
    static String GetCursorPercentsT(const Channel &source);
};
