// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors_.h"


class Display
{
public:

    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static void BeginFrame(const Color &color);

    static void EndFrame();

    static void Update();
};
