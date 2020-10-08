#pragma once
#include "common/Display/Colors.h"
#include "common/Utils/String.h"


class Char
{
public:
    Char(char s) : symbol(s) {}
    int Draw(int x, int y, Color::E color = Color::NUM);
private:
    char symbol;
};


class Text
{
public:
    Text(char *t) : text(t) {}
    Text(pString t) : text(t) {};
    int Draw(int x, int y, Color::E color = Color::NUM);
    void DrawInRect(int x, int y, int width, int height);
private:
    String text;
    int DrawChar(int x, int y, uint8 symbol);
};
