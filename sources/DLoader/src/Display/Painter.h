#pragma once
#include "common/Display/Colors_c.h"
#include "DisplayTypes.h"
#include "PainterText.h"


namespace Painter
{
    void SetPoint(int, int);
    
    void BeginScene(Color::E color);

    void CalculateCurrentColor();

    void EndScene(void);

    void DrawMultiVPointLine(int numLines, int y, uint16 *x, int delta, int count, Color::E color);

    void DrawMultiHPointLine(int numLines, int x, uint8 *y, int delta, int count, Color::E color);

    void SetBrightnessDisplay(int16 brightness);  // ”становить €ркость диспле€.
    
    void DrawCharHardCol(int, int, char);
};
