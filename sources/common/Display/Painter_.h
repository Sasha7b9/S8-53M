#pragma once
#include "common/Display/Colors_.h"
#include "common/Display/Font/Font_.h"


class Painter 
{
public:

    static void BeginScene(const Color &color);

    static void EndScene(bool endScene = true);

    // �� ��������� ����������� ������ ����� � ��������
    static void SendFrame(bool noFonts = false);

    // ���������� ������� �������.
    static void SetBrightnessDisplay(int16 brightness);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    // modeLines - true - �������, false - �������
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void CalculateCurrentColor();

    static bool noFonts;

    static void LoadPalette();

private:

    static void RunDisplay();

    static Color GetColor(int x, int y);
};
