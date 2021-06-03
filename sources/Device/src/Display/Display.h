// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tables.h"
#include "common/Display/Colors_.h"


struct DataSettings;

struct DrawMode { enum E {
    Default,        // Стандартная отрисовка в цикле
    Timer           // Отрисовка по таймеру заданной функции
}; };


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

    static void RotateTrigLev();

    static void Redraw();

    static void SetDrawMode(DrawMode::E mode, pFuncVV func = nullptr);

    static void SetAddDrawFunction(pFuncVV func);

    static void RemoveAddDrawFunction();

    static void Clear();

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers();

    // После отрисовки очередного экрана эта функция будет вызвана один раз.
    static void RunAfterDraw(pFuncVV func);

    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

    static int DrawScaleLine(int x, bool for_trig_level);

    struct Message
    {
        static void Show(pchar text);

    private:

        static void FuncDraw();

        static pchar text;
    };

private:

    static void FuncOnTimerDisableShowLevelTrigLev();

    static void FuncOnTimerRShiftMarkersAutoHide();

    static bool NeedForClearScreen();

    static void DrawTimeForFrame(uint timeMS);

    // Возвращает true, если надо обновлять дисплей
    static bool NeedUpdate();
};
