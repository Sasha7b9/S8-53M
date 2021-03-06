// 2021/06/30 15:45:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"


class Display
{
public:

    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static void BeginFrame(const Color &color);

    static void EndFrame();

    static void Update();

    static bool IsRunning();

private:

    static void DrawMessage(pchar message1, pchar message2 = nullptr);

    static void DrawProgressBar();

    static void DrawBigMNIPI();
};
