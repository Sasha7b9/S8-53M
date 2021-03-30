#pragma once
#include "Tables.h"
#include "common/Display/Colors_.h"


struct DataSettings;

struct DrawMode
{
    enum E
    {
        Auto,
        Hand
    };
};


class Display
{
public:

    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static uint8 *GetBuffer();

    static uint8 *GetBufferEnd();

    static void BeginFrame(const Color &color);

    static void EndFrame();

    static void Update();

    static void RotateRShift(const Channel &ch);

    static void RotateTrigLev();

    static void Redraw();

    static void SetDrawMode(DrawMode::E mode, pFuncVV func);

    static void SetAddDrawFunction(pFuncVV func);

    static void RemoveAddDrawFunction();

    static void Clear();

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers();

    // ����� ��������� ���������� ������ ��� ������� ����� ������� ���� ���.
    static void RunAfterDraw(pFuncVV func);

    // ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftA();

    // ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftB();

    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

    static int DrawScaleLine(int x, bool for_trig_level);

private:

    // ���������� ������ �������� �� �������.
    static void DrawCursorTShift();

    static void FuncOnTimerDisableShowLevelRShiftA();

    static void FuncOnTimerDisableShowLevelRShiftB();

    static void FuncOnTimerDisableShowLevelTrigLev();

    static void FuncOnTimerRShiftMarkersAutoHide();

    static bool NeedForClearScreen();

    static void DrawTimeForFrame(uint timeMS);
};
