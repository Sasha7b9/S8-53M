#pragma once
#include "common/Utils/String_.h"


// ��������� ��������� ���������.
struct SettingsCursors
{
    CursCntrl       cntrlU[NumChannels];    // �������� ������� ����������.
    CursCntrl       cntrlT[NumChannels];    // �������� ������� ����������.
    Channel::E      source;                 // �������� - � ������ ������ ��������� �������.
    float           posU[NumChannels][2];   // ������� ������� �������� ���������� ����� �������.
    float           posT[NumChannels][2];   // ������� ������� �������� ������� ����� �������.
    float           dU_100percents[2];      // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float           dT_100percents[2];      // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    CursMovement    movement;               // ��� ������������ �������� - �� ������ ��� �� ���������.
    CursActive      active;                 // ����� ������� ������ �������.
    CursLookMode    look_mode[2];           // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool            show_freq;              // ������������� � true ��������, ��� ����� ���������� �� ������ �������� 1/dT ����� ���������.
    bool            show;                   // ���������� �� �������.
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
