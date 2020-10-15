#pragma once
#include "common/Display/Colors_c.h"
#include "DisplayTypes.h"
#include "PainterText.h"


namespace Painter
{
    void SetPoint(int, int);
    
    void SendToDisplay(uint8 *bytes, int numBytes);

    void BeginScene(Color::E color);

    void EndScene(void);

    void ResetFlash(void);

    void SetColor(Color::E color);

    void LoadPalette(int num);

    static void DrawHLine(int y, int x0, int x1);

    static void DrawVLine(int x, int y0, int y1);

    void DrawVPointLine(int x, int y0, int y1, float delta, Color::E color);

    void DrawHPointLine(int y, int x0, int x1, float delta);

    void DrawMultiVPointLine(int numLines, int y, uint16 *x, int delta, int count, Color::E color);

    void DrawMultiHPointLine(int numLines, int x, uint8 *y, int delta, int count, Color::E color);

    void DrawLine(int x0, int y0, int x1, int y1);
    // Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами
    // Линия всегда начинается со штриха. dStart указывает смещение первой рисуемой точки относительно начала штриха.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    // Рисует прерывистую вертикальную линию.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height);

    void FillRegion(int x, int y, int width, int height);

    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade);

    void SetBrightnessDisplay(int16 brightness);  // Установить яркость дисплея.
    
    // Нарисовать массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color::E color);
    // modeLines - true - точками, false - точками
    void DrawSignal(int x, uint8 data[281], bool modeLines);
    
    void CalculateCurrentColor();

    void DrawCharHardCol(int, int, char);
};
