#pragma once
#include "common/Display/Colors_c.h"
#include "common/Display/Font/Font_c.h"


class Painter 
{
public:

    static void BeginScene(Color::E color);

    static void EndScene(bool endScene = true);

    // По умолчанию запрашиваем первый фрейм с шрифтами
    static void SendFrame(bool noFonts = false);

    // Установить яркость дисплея.
    static void SetBrightnessDisplay(int16 brightness);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    // modeLines - true - точками, false - точками
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void CalculateCurrentColor();

    static bool noFonts;

    static void LoadPalette();

private:

    static void RunDisplay();

    static Color::E GetColor(int x, int y);
};
