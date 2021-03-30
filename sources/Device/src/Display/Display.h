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

    // После отрисовки очередного экрана эта функция будет вызвана один раз.
    static void RunAfterDraw(pFuncVV func);

    // Отключить вспомогательную линию маркера смещения по напряжению первого канала.
    static void DisableShowLevelRShiftA();

    // Отключить вспомогательную линию маркера смещения по напряжению второго канала.
    static void DisableShowLevelRShiftB();

    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

    static int DrawScaleLine(int x, bool for_trig_level);

private:

    static void ShowWarn(pchar message);

    // Нарисовать маркер смещения по времени.
    static void DrawCursorTShift();

    // Написать сообщения отладочной консоли.
    static void DrawConsole();

    // Написать предупреждения.
    static void DrawWarnings();

    static int CalculateFreeSize();

    static void FuncOnTimerDisableShowLevelRShiftA();

    static void FuncOnTimerDisableShowLevelRShiftB();

    static void FuncOnTimerDisableShowLevelTrigLev();

    static void FuncOnTimerRShiftMarkersAutoHide();

    static void OnTimerShowWarning();

    static void DrawStringInRectangle(int x, int y, char const *text);

    static void DrawStringNavigation();

    static int FirstEmptyString();

    static bool NeedForClearScreen();

    static void DrawTimeForFrame(uint timeMS);

    static void DeleteFirstString();

    static void AddString(pchar string);
};

