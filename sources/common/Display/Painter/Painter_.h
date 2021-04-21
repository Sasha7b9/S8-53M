// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Display/Font/Font_.h"


class Painter 
{
public:

    // По умолчанию запрашиваем первый фрейм с шрифтами
    static void SendFrame(bool noFonts = false);

    // Установить яркость дисплея.
    static void SetBrightnessDisplay(int16 brightness);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void CalculateCurrentColor();

    static bool noFonts;

    static void LoadPalette();

private:

    static void RunDisplay();

    static Color GetColor(int x, int y);
};
