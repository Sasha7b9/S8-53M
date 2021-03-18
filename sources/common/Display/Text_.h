#pragma once
#include "common/Display/Colors_.h"
#include "common/Utils/String_.h"


class Char
{
public:

    Char(char s) : symbol(s) {}

    int Draw(int x, int y, Color color = Color::Count);

    void Draw2SymbolsInPosition(int x, int y, char symbol2, Color color1, Color color2);

    void Draw4SymbolsInRect(int x, int y, Color color = Color::Count);

    void Draw10SymbolsInRect(int x, int y);

private:
    char symbol;
};


class Text
{
public:

    Text(pchar t) : text(t) {};

    Text(const String &string) : text(string) {}

    ~Text();

    int Draw(int x, int y, Color color = Color::Count);

    void DrawInRect(int x, int y, uint width, uint height);

    void DrawBig(int x, int y, uint size);

    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::Count);

    void DrawRelativelyRight(int xRight, int y, Color color = Color::Count);

    // Выводит текст на прямоугольнике цвета colorBackgound
    int DrawOnBackground(int x, int y, Color colorBackground);

    int DrawWithLimitation(int x, int y, Color color, int limitX, int limitY, int limitWidth, int limitHeight);

    // Возвращает нижнюю координату прямоугольника.
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill);

    int DrawInRectWithTransfers(int x, int y, int width, int height, Color color = Color::Count);

    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground.
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);

    int DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);

private:
    String text;
    int DrawChar(int x, int y, uint8 symbol);
};
