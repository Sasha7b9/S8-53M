#pragma once
#include "common/Display/Colors_c.h"
#include "common/Display/DisplayTypes_c.h"
#include "common/Display/Font/Font_c.h"


struct StateTransmit { enum E
{
    Free,
    NeedForTransmitFirst,  // ��� ����� ����� �������� ������ ���� - ���������� ������
    NeedForTransmitSecond, // ��� ����� ����� �������� ������ � ����������� ����� - ������ �� ����������
    InProcess
};};


class Painter 
{
public:

    static void Init();

    static void BeginScene(Color::E color);

    static void EndScene(bool endScene = true);
    // �� ��������� ����������� ������ ����� � ��������
    static void SendFrame(bool first, bool noFonts = false);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade);
    // ���������� ������� �������.
    static void SetBrightnessDisplay(int16 brightness);

    static uint16 ReduceBrightness(uint16 colorValue, float newBrightness);
    // modeLines - true - �������, false - �������
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static int DrawFormatText(int x, int y, Color::E color, char *text, ...);

    // ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground.
    static void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, Color::E colorBackground);

    static int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill);

    static void DrawHintsForSmallButton(int x, int y, int width, void *smallButton);

    static void Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2);

    static void CalculateCurrentColor();

    static bool noFonts;

private:

    static void RunDisplay();

    static Color::E GetColor(int x, int y);

    static void Get8Points(int x, int y, uint8 buffer[4]);

    static int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

    static StateTransmit::E stateTransmit;
};
