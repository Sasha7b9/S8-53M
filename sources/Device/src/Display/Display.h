#pragma once
#include "Tables.h"


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

    static int topMeasures;     // ���� ������� ������ ���������. ��� �������� ����� ��� ����������� ������ ��������� ��
                                // ����� - ����� ��� ������ �� �����������
    static void Init();

    static uint8 *GetBuffer();

    static uint8 *GetBufferEnd();

    static void BeginFrame(const Color &color);

    static void EndFrame();

    static void Update();

    static void RotateRShift(const Channel &ch);

    static void RotateTrigLev();

    static void Redraw();

    static void EnableTrigLabel(bool enable);

    static void ShowWarningBad(Warning::E warning);

    static void ShowWarningGood(Warning::E warning);

    static void ClearFromWarnings();

    static void SetDrawMode(DrawMode::E mode, pFuncVV func);

    static void SetAddDrawFunction(pFuncVV func);

    static void RemoveAddDrawFunction();

    static void Clear();

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers();

    static void AddStringToIndicating(pchar string);

    static void OneStringUp();

    static void OneStringDown();

    static void SetPauseForConsole(bool pause);

    // ����� ��������� ���������� ������ ��� ������� ����� ������� ���� ���.
    static void RunAfterDraw(pFuncVV func);

    // ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftA();

    // ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftB();

    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

private:

    static void ShowWarn(pchar message);

    // ���������� �����.
    static void DrawGrid(int left, int top, int width, int height);

    // ���������� ������ �����.
    static void DrawFullGrid();

    static void DrawCursorsWindow();

    static void DrawCursorsRShift();

    // ���������� ������� �������� �� ����������
    static void DrawCursorRShift(const Channel &ch);

    // ���������� ������ ������ �������������.
    static void DrawCursorTrigLevel();

    // ���������� ������ �������� �� �������.
    static void DrawCursorTShift();

    // �������� ���������� ��� ������ - � ������ ����� �������.
    static void DrawLowPart();

    static void DrawHiPart();

    static void DrawHiRightPart();

    // ������� ������� �����.
    static void DrawTime(int x, int y);

    // ���������� ������� ��������� ���������.
    static void DrawCursors();

    // ������� �������� �������� ��������� ���������.
    static void WriteCursors();

    // ������� �������� �������������� ���������.
    static void DrawMeasures();

    // �������� ��������� ���������� �������.
    static void DrawConsole();

    // �������� ��������������.
    static void DrawWarnings();

    static int CalculateCountV();

    static int CalculateFreeSize();

    static void DrawGridSpectrum();

    static void DrawScaleLine(int x, bool forTrigLev);

    static void WriteTextVoltage(const Channel &ch, int x, int y);

    static void FuncOnTimerDisableShowLevelRShiftA();

    static void FuncOnTimerDisableShowLevelRShiftB();

    static void FuncOnTimerDisableShowLevelTrigLev();

    static void FuncOnTimerRShiftMarkersAutoHide();

    static void OnTimerShowWarning();

    static void DrawStringInRectangle(int x, int y, char const *text);

    static void DrawStringNavigation();

    static int FirstEmptyString();

    static bool NeedForClearScreen();

    static void WriteStringAndNumber(pchar text, int x, int y, int number);

    static void DrawTimeForFrame(uint timeMS);

    static void DeleteFirstString();
    // ���������� �������������� ������ ��������� ���������
    // y         - �������� �������� �������
    // x_tearing - ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������
    static void DrawHorizontalCursor(int y, int x_tearing);

    // ���������� ������������ ������ ��������� ���������.
    // x         - �������� �������� �������
    // y_tearing - ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������
    static void DrawVerticalCursor(int x, int yTearing);

    // ������� �������� ������ �������������. 
    static void WriteValueTrigLevel();

    static void AddString(pchar string);

    static int CalculateCountH();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX,
        float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX,
        int deltaY, int stepX, int stepY);
};

