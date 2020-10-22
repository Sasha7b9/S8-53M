#pragma once
#include "common/Display/Colors_c.h"
#include "DisplayTypes.h"
#include "PainterText.h"


namespace Painter
{
    void SetPoint(int, int);
    
    void BeginScene(Color::E color);

    void EndScene(void);

    static void DrawVLine(int x, int y0, int y1);

    void DrawVPointLine(int x, int y0, int y1, float delta, Color::E color);

    void DrawHPointLine(int y, int x0, int x1, float delta);

    void DrawMultiVPointLine(int numLines, int y, uint16 *x, int delta, int count, Color::E color);

    void DrawMultiHPointLine(int numLines, int x, uint8 *y, int delta, int count, Color::E color);

    void SetBrightnessDisplay(int16 brightness);  // ”становить €ркость диспле€.
    
    // Ќарисовать массив вертикальных линий. Ћинии рисуютс€ одна за другой. y0y1 - массив вертикальных координат
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color::E color);
    // modeLines - true - точками, false - точками
    void DrawSignal(int x, uint8 data[281], bool modeLines);
    
    void CalculateCurrentColor();

    void DrawCharHardCol(int, int, char);
};
