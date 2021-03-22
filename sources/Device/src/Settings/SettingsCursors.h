#pragma once
#include "common/Utils/String_.h"


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
    bool            show_freq;                  // ������������� � true ��������, ��� ����� ���������� �� ������ �������� 1/dT ����� ���������.
    bool            show;                       // ���������� �� �������.
};


struct Cursors
{
    // �������� ������� ������� ����������.
    static float GetPosU(const Channel &ch, int num);

    // ���������� true,���� ����� �������� �������.
    static bool NecessaryDraw();

    // �������� ������ ������� ����������.
    static String GetVoltage(const Channel &source, int numCur);

    // �������� ������ ������� �������.
    static String GetTime(const Channel &source, int numCur);

    // �������� ������ ��������� �������� ����������.
    static String GetPercentsU(const Channel &source);

    // �������� ������ ��������� �������� �������.
    static String GetPercentsT(const Channel &source);

    // ���������� ������� ��������� ���������.
    static void Draw();

private:

    // ���������� ������������ ������ ��������� ���������.
    // x         - �������� �������� �������
    // y_tearing - ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������
    static void DrawVertical(int x, int yTearing);

    // ���������� �������������� ������ ��������� ���������
    // y         - �������� �������� �������
    // x_tearing - ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������
    static void DrawHorizontal(int y, int x_tearing);
};
