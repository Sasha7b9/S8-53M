// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color;


class Display
{
public:
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static void Update();

    static bool IsRun();

    static void BeginFrame(const Color &color);

    static void EndFrame();

    bool isRun;
    float value;
    float direction;
    uint timePrev;
};
