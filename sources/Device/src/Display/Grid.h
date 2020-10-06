#pragma once


class Grid
{
public:
    static const int DELTA = 20;
    static const int TOP = 19;
    static const int WIDTH = 280;
    static const int CELLS_IN_HEIGHT = 10;


    static int Delta();
    static int Left();
    static int Right();
    static int Width();
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
};
