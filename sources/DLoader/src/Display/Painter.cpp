#include "Painter.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


static bool inverseColors = false;
static Color::E currentColor = Color::Count;
//static bool framesElapsed = false;
static int numberColorsUsed = 0;

static enum StateTransmit
{
    StateTransmit_Free,
    StateTransmit_NeedForTransmitFirst,  // Это когда нужно передать первый кадр - передаются шрифты
    StateTransmit_NeedForTransmitSecond, // Это когда нужно передать второй и последующий кадры - шрифты не передаются
    StateTransmit_InProcess
} stateTransmit = StateTransmit_Free;



void Painter_SendFrame(bool first)
{
    if (stateTransmit == StateTransmit_Free)
    {
        stateTransmit = (first ? StateTransmit_NeedForTransmitFirst : StateTransmit_NeedForTransmitSecond);
    }
}


void CalculateCurrentColor(void)
{
    if (currentColor == Color::FLASH_10)
    {
        Painter_SetColor(inverseColors ? gColorBack : gColorFill);
    }
    else if (currentColor == Color::FLASH_01)
    {
        Painter_SetColor(inverseColors ? gColorFill : gColorBack);
    }
}


void CalculateColor(uint8 *color)
{
    currentColor = (Color::E)*color;
    if (*color == Color::FLASH_10)
    {
        *color = static_cast<uint8>(inverseColors ? gColorBack : gColorFill);
    }
    else if (*color == Color::FLASH_01)
    {
        *color = static_cast<uint8>(inverseColors ? gColorFill : gColorBack);
    }
}


void InverseColor(Color::E *color)
{
    *color = (*color == Color::BLACK) ? Color::WHITE : Color::BLACK;
}


static void OnTimerFlashDisplay(void)
{
    inverseColors = !inverseColors;
}


void Painter_ResetFlash(void)
{
    Timer_Enable(kFlashDisplay, 400, OnTimerFlashDisplay);
    inverseColors = false;
}


void Painter_DrawRectangle(int x, int y, int width, int height)
{
    Painter_DrawHLine(y, x, x + width);
    Painter_DrawVLine(x, y, y + height);
    Painter_DrawHLine(y + height, x, x + width);
    if (x + width < SCREEN_WIDTH)
    {
        Painter_DrawVLine(x + width, y, y + height);
    }
}


void Painter_DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            Painter_DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        Painter_DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}


void Painter_SetFont(TypeFont::E)
{
}


void Painter_LoadFont(TypeFont::E)
{
}


void Painter_DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            Painter_DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        Painter_DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}


void Painter_SendToDisplay(uint8 *, int)
{
}


void Painter_SetPalette(Color::E)
{
}


void Painter_SetColor(Color::E)
{
}


void Painter_DrawHLine(int, int, int)
{
}


void Painter_DrawVLine(int, int, int)
{
}


void Painter_DrawVPointLine(int, int, int, float, Color::E)
{
}


void Painter_DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += static_cast<int>(delta))
    {
        Painter_SetPoint(x, y);
    }
}


void Painter_SetPoint(int, int )
{
}


void Painter_DrawMultiVPointLine(int , int , uint16 [], int , int , Color::E ) 
{
}


void Painter_DrawMultiHPointLine(int , int , uint8 [], int , int , Color::E )
{
}


void Painter_DrawLine(int x0, int y0, int x1, int y1)
{
    if (x0 == x1)
    {
        Painter_DrawVLine(x0, y0, y1);
    }
    else if (y0 == y1)
    {
        Painter_DrawHLine(y0, x0, x1);
    }
}


void Painter_FillRegion(int , int , int , int )
{
}


void Painter_DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade)
{
    if (inShade)
    {
        thickness = 1;
    }
    Painter_FillRegionC(x + thickness, y + thickness, width - thickness * 2, height - thickness * 2, normal);
    if (isPressed || inShade)
    {
        for (int i = 0; i < thickness; i++)
        {
            Painter_DrawHLineC(y + i, x + i, x + width - i, dark);
            Painter_DrawVLine(x + i, y + 1 + i, y + height - i);
            Painter_DrawVLineC(x + width - i, y + 1 + i, y + height - i, bright);
            Painter_DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            Painter_DrawHLineC(y + i, x + i, x + width - i, bright);
            Painter_DrawVLine(x + i, y + 1 + i, y + height - i);
            Painter_DrawVLineC(x + width - i, y + 1 + i, y + height - i, dark);
            Painter_DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
}


void Painter_SetBrightnessDisplay(int16)
{
}


int NumberColorsInSceneCol()
{
    return numberColorsUsed;
}


void Painter_DrawVLineArray(int , int , uint8 *, Color::E )
{
}


void Painter_DrawSignal(int , uint8 [281], bool )
{
}


void Painter_LoadPalette(int num)
{
    int min[] = {0, 5, 10};
    int max[] = {4, 9, 15};

    int i = min[num];
    int a = max[num];

    for (; i <= a; i++)
    {
        Painter_SetPalette((Color::E)i);
    }
}


void Painter_BeginScene(Color::E color)
{
    if (stateTransmit == StateTransmit_NeedForTransmitFirst || stateTransmit == StateTransmit_NeedForTransmitSecond)
    {
        bool needForLoadFontsAndPalette = stateTransmit == StateTransmit_NeedForTransmitFirst;
        stateTransmit = StateTransmit_InProcess;
        if(needForLoadFontsAndPalette) 
        {
            Painter_LoadPalette(0);
            Painter_LoadPalette(1);
            Painter_LoadPalette(2);
            Painter_LoadFont(TypeFont::_5);
            Painter_LoadFont(TypeFont::_8);
            Painter_LoadFont(TypeFont::_UGO);
            Painter_LoadFont(TypeFont::_UGO2);
        }
    }

    Painter_FillRegionC(0, 0, 319, 239, color);
}


void Painter_EndScene()
{
}


Color::E GetColor(int , int )
{
    return Color::WHITE;
}


void Get8Points(int , int , uint8 [4])
{
}


uint8 Get2Points(int, int)
{
    return 0;
}


void Painter_DrawPicture(int , int , int , int , uint8 *)
{
}


uint16 Painter_ReduceBrightness(uint16 colorValue, float newBrightness)
{
    int red = static_cast<int>(static_cast<float>(R_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(red, red, 0, 31);
    int green = static_cast<int>(static_cast<float>(G_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(green, green, 0, 63);
    int blue = static_cast<int>(static_cast<float>(B_FROM_COLOR(colorValue)) * newBrightness);
    LIMITATION(blue, blue, 0, 31);
    return MAKE_COLOR(red, green, blue);
}
