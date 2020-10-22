#pragma once
#include "DisplayTypes.h"
#include "common/Display/Font/Font_c.h"


namespace Painter
{
    void SetFont(TypeFont::E typeFont);
    // ��������� ������ � �������
    static void LoadFont(TypeFont::E typeFont);

    int DrawStringInCenterRect(int x, int y, int width, int height, const char *text);

    int DrawStringInCenterRectC(int x, int y, int width, int height, const char *text, Color::E color);
    // ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, Color::E colorBackground);

    int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill);

    void DrawTextInRect(int x, int y, int width, char *text);

    void DrawTextRelativelyRight(int xRight, int y, const char *text);

    void DrawBigText(int x, int y, int size, const char *text);

    void DrawBigTextInBuffer(int x, int y, int size, const char *text, uint8 buffer[320][240]);
};
