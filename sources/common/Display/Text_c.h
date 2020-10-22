#pragma once
#include "common/Display/Colors_c.h"
#include "common/Utils/String_c.h"


class Char
{
public:
    Char(char s) : symbol(s) {}
    int Draw(int x, int y, Color::E color = Color::Count);
private:
    char symbol;
};


class Text
{
public:
    Text(char *t) : text(t) {}
    Text(pString t) : text(t) {};
    int Draw(int x, int y, Color::E color = Color::Count);
    void DrawInRect(int x, int y, int width, int height);
    void DrawBig(int x, int y, int size);
private:
    String text;
    int DrawChar(int x, int y, uint8 symbol);
};
