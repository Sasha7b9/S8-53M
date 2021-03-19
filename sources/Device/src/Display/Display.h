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

    static int topMeasures;     // ¬ерх таблицы вывода измерений. Ёто значение нужно дл€ нормального вывода сообщений на
                                // экран - чтобы они ничего не перекрывали
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

    // ѕосле отрисовки очередного экрана эта функци€ будет вызвана один раз.
    static void RunAfterDraw(pFuncVV func);

    // ќтключить вспомогательную линию маркера смещени€ по напр€жению первого канала.
    static void DisableShowLevelRShiftA();

    // ќтключить вспомогательную линию маркера смещени€ по напр€жению второго канала.
    static void DisableShowLevelRShiftB();

    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

private:

    static void ShowWarn(pchar message);

    // Ќарисовать сетку.
    static void DrawGrid(int left, int top, int width, int height);

    // Ќарисовать полную сетку.
    static void DrawFullGrid();

    static void DrawCursorsWindow();

    static void DrawCursorsRShift();

    // Ќарисовать маркеры смещени€ по напр€жению
    static void DrawCursorRShift(const Channel &ch);

    // Ќарисовать маркер уровн€ синхронизации.
    static void DrawCursorTrigLevel();

    // Ќарисовать маркер смещени€ по времени.
    static void DrawCursorTShift();

    // Ќаписать информацию под сеткой - в нижней части диспле€.
    static void DrawLowPart();

    static void DrawHiPart();

    static void DrawHiRightPart();

    // ¬ывести текущее врем€.
    static void DrawTime(int x, int y);

    // Ќарисовать курсоры курсорных измерений.
    static void DrawCursors();

    // ¬ывести значени€ курсоров курсорных измерений.
    static void WriteCursors();

    // ¬ывести значени€ автоматических измерений.
    static void DrawMeasures();

    // Ќаписать сообщени€ отладочной консоли.
    static void DrawConsole();

    // Ќаписать предупреждени€.
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
    // Ќарисовать горизонтальный курсор курсорных измерений
    // y         - числовое значение курсора
    // x_tearing - координата места, в котором необходимо сделать разрыв дл€ квадрата пересечени€
    static void DrawHorizontalCursor(int y, int x_tearing);

    // Ќарисовать вертикальный курсор курсорных измерений.
    // x         - числовое значение курсора
    // y_tearing - координата места, в котором необходимо сделать разрыв дл€ квадрата пересечени€
    static void DrawVerticalCursor(int x, int yTearing);

    // ¬ывести значение уровн€ синхронизации. 
    static void WriteValueTrigLevel();

    static void AddString(pchar string);

    static int CalculateCountH();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX,
        float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX,
        int deltaY, int stepX, int stepY);
};

