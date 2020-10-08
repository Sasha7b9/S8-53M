#pragma once
#include "Colors.h"
#include "DisplayTypes.h"
#include "PainterC.h"


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

    static void SendToDisplay(uint8 *bytes, int numBytes);

    static void SendToVCP(pUCHAR pointer, int size);

    static void BeginScene(Color::E color);

    static void EndScene(bool endScene = true);
    // �� ��������� ����������� ������ ����� � ��������
    static void SendFrame(bool first, bool noFonts = false);

    static void ResetFlash();

    static void SetColor(Color::E color);

    static Color::E CurrentColor();

    static void LoadPalette();

    static void SetPalette(Color::E color);

    static void DrawVPointLine(int x, int y0, int y1, float delta, Color::E color);

    static void DrawHPointLine(int y, int x0, int x1, float delta);

    static void DrawLine(int x0, int y0, int x1, int y1);

    static void DrawLineC(int x0, int y0, int x1, int y1, Color::E color);
    // \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������.
    // ����� ������ ���������� �� ������. dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    //  ������ ����������� ������������ �����.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade);
    // ���������� ������� �������.
    static void SetBrightnessDisplay(int16 brightness);

    static uint16 ReduceBrightness(uint16 colorValue, float newBrightness);
    // ���������� ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
    static void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color::E color);
    // modeLines - true - �������, false - �������
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void SetFont(TypeFont::E typeFont);
    // ��������� ������ � �������.
    static void LoadFont(TypeFont::E typeFont);

    static int DrawChar(int x, int y, char symbol);

    static int DrawCharC(int x, int y, char symbol, Color::E color);

    static int DrawText(int x, int y, const char *text);

    static int DrawTextC(int x, int y, const char *text, Color::E color);
    // ������� ����� �� �������������� ����� colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color::E colorBackground);

    static int DrawTextWithLimitationC(int x, int y, const char* text, Color::E color, int limitX, int limitY, int limitWidth, int limitHeight);
    // ���������� ������ ���������� ��������������.
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color::E colorBackground, Color::E colorFill);

    static int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color::E color);

    static int DrawFormatText(int x, int y, Color::E color, char *text, ...);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text);

    static int DrawStringInCenterRectC(int x, int y, int width, int height, const char *text, Color::E color);
    // ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground.
    static void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, Color::E colorBackground);

    static int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill);

    static void DrawHintsForSmallButton(int x, int y, int width, void *smallButton);

    static void DrawTextInRect(int x, int y, int width, int height, char *text);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text);

    static void DrawTextRelativelyRightC(int xRight, int y, const char *text, Color::E color);

    static void Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2);

    static void Draw4SymbolsInRect(int x, int y, char eChar);

    static void Draw4SymbolsInRectC(int x, int y, char eChar, Color::E color);

    static void Draw10SymbolsInRect(int x, int y, char eChar);
    // ����� ����� � ����������.
    static int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    static void DrawBigText(int x, int y, int size, const char *text);

private:

    static void RunDisplay();

    static void CalculateCurrentColor();

    static Color::E GetColor(int x, int y);

    static void Get8Points(int x, int y, uint8 buffer[4]);

    static void DrawCharHardCol(int x, int y, char symbol);

    static int DrawCharWithLimitation(int eX, int eY, uchar symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    static int DrawSubString(int x, int y, char *text);

    static int DrawSpaces(int x, int y, char *text, int *numSymbols);

    static int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

    static bool GetHeightTextWithTransfers(int left, int top, int right, const char *text, int *height);
};
