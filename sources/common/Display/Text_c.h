#pragma once
#include "common/Display/Colors_c.h"
#include "common/Utils/String_c.h"


class Char
{
public:
    Char(char s) : symbol(s) {}
    int Draw(int x, int y, Color::E color = Color::Count);
    void Draw4SymbolsInRect(int x, int y, Color::E color = Color::Count);
    void Draw10SymbolsInRect(int x, int y);

private:
    char symbol;
};


class Text
{
public:
    Text(char *format, ...);
    Text(pString t) : text(t) {};
    int Draw(int x, int y, Color::E color = Color::Count);
    void DrawInRect(int x, int y, int width, int height);
    void DrawBig(int x, int y, int size);
    int DrawInCenterRect(int x, int y, int width, int height, Color::E color = Color::Count);
    void DrawRelativelyRight(int xRight, int y, Color::E color = Color::Count);
    // Выводит текст на прямоугольнике цвета colorBackgound
    int DrawOnBackground(int x, int y, Color::E colorBackground);
    int DrawWithLimitation(int x, int y, Color::E color, int limitX, int limitY, int limitWidth, int limitHeight);
    // Возвращает нижнюю координату прямоугольника.
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color::E colorBackground, Color::E colorFill);
    int DrawInRectWithTransfers(int x, int y, int width, int height, Color::E color = Color::Count);
    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground.
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color::E colorText, int widthBorder, Color::E colorBackground);
    int DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color::E colorBackground, Color::E colorFill);

private:
    String text;
    int DrawChar(int x, int y, uint8 symbol);
};
