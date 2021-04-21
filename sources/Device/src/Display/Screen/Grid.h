// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Grid
{
public:
    static const int DELTA = 20;
    static const int TOP = 19;
    static const int WIDTH = 280;
    static const int CELLS_IN_HEIGHT = 10;

    static void Draw();

    static int Delta();
    static int Left();
    static int Right();
    static int Width();
    static int Height();
    static float DeltaY();
    static float DeltaX();
    static int Bottom();

    static int WidthInCells();

    static int FullBottom();
    static int FullHeight();
    static int FullCenterHeight();

    static int ChannelBottom();
    static int ChannelHeight();
    static int ChannelCenterHeight();

    static int MathHeight();
    static int MathBottom();
    static int MathTop();

    static int BottomMessages();

private:
    static int SignalWidth();

    static void Draw(int left, int top, int width, int height);

    static void DrawSpectrum();

    static void DrawType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX,
        float deltaY, float stepX, float stepY);

    static void DrawType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX,
        int deltaY, int stepX, int stepY);

    static int CalculateCountV();

    static int CalculateCountH();
};
