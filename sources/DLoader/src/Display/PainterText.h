#pragma once
#include "common/Display/Font/Font_c.h"


namespace Painter
{
    void DrawTextInRect(int x, int y, int width, char *text);

    void DrawTextRelativelyRight(int xRight, int y, const char *text);

    void DrawBigText(int x, int y, int size, const char *text);

    void DrawBigTextInBuffer(int x, int y, int size, const char *text, uint8 buffer[320][240]);
};
