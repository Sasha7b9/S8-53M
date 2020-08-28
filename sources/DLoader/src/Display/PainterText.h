#pragma once
#include "defines.h"
#include "DisplayTypes.h"


void Painter_SetFont(TypeFont::E typeFont);
/// ��������� ������ � �������
void Painter_LoadFont(TypeFont::E typeFont);

int Painter_DrawChar(int x, int y, char symbol);

int Painter_DrawCharC(int x, int y, char symbol, Color::E color);

int Painter_DrawText(int x, int y, const char *text);

int Painter_DrawTextC(int x, int y, const char *text, Color::E color);

int Painter_DrawTextWithLimitationC(int x, int y, const char* text, Color::E color, int limitX, int limitY, int limitWidth, int limitHeight);

int Painter_DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color::E color);

int Painter_DrawTextFormatting(int x, int y, Color::E color, char *text, ...);

int Painter_DrawStringInCenterRect(int x, int y, int width, int height, const char *text);

int Painter_DrawStringInCenterRectC(int x, int y, int width, int height, const char *text, Color::E color);
/// ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
void Painter_DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color::E colorText, int widthBorder, Color::E colorBackground);

int Painter_DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color::E colorBackground, Color::E colorFill);

void Painter_DrawTextInRect(int x, int y, int width, char *text);

void Painter_DrawTextRelativelyRight(int xRight, int y, const char *text);

void Painter_Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color::E color1, Color::E color2);

void Painter_Draw4SymbolsInRect(int x, int y, char eChar);

void Painter_Draw10SymbolsInRect(int x, int y, char eChar);
/// ����� ����� � ����������
int Painter_DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

void Painter_DrawBigText(int x, int y, int size, const char *text);

void Painter_DrawBigTextInBuffer(int x, int y, int size, const char *text, uint8 buffer[320][240]);
