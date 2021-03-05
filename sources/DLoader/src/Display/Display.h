#pragma once


struct Color;


class Display
{
public:
    static const int WIDTH = 320;

    static void Init();

    static void Update(void);

    static bool IsRun(void);

    static void BeginFrame(const Color &color);

    static void EndFrame();

    bool isRun;
    float value;
    float direction;
    uint timePrev;
};
