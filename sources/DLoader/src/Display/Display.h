#pragma once


class Display
{
public:
    static const int WIDTH = 320;

    static void Init();

    static void Update(void);

    static bool IsRun(void);

    bool isRun;
    float value;
    float direction;
    uint timePrev;
};
