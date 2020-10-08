#pragma once
#include "defines.h"


class Display
{
public:
    static const int WIDTH = 320;

    bool isRun;
    float value;
    float direction;
    uint timePrev;
};


void Display_Init(void);

void Display_Update(void);

void Display_Update1(void);

bool Display_IsRun(void);
