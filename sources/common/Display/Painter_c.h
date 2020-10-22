#pragma once
#include "common/Display/Colors_c.h"
#include "common/Display/DisplayTypes_c.h"
#include "common/Display/Font/Font_c.h"


struct StateTransmit { enum E
{
    Free,
    NeedForTransmitFirst,  // Это когда нужно передать первый кадр - передаются шрифты
    NeedForTransmitSecond, // Это когда нужно передать второй и последующий кадры - шрифты не передаются
    InProcess
};};


class Painter 
{
public:

    static void Init();

    static void BeginScene(Color::E color);

    static void EndScene(bool endScene = true);
    // По умолчанию запрашиваем первый фрейм с шрифтами
    static void SendFrame(bool first, bool noFonts = false);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade);
    // Установить яркость дисплея.
    static void SetBrightnessDisplay(int16 brightness);

    static uint16 ReduceBrightness(uint16 colorValue, float newBrightness);
    // modeLines - true - точками, false - точками
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void CalculateCurrentColor();

    static bool noFonts;

private:

    static void RunDisplay();

    static Color::E GetColor(int x, int y);

    static StateTransmit::E stateTransmit;
};
