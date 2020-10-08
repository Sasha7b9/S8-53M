#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


using namespace Primitives;


static bool inverseColors = false;
static Color::E currentColor = Color::NUM;
static int numberColorsUsed = 0;


struct StateTransmit { enum E
{
    Free,
    NeedForTransmitFirst,  // Это когда нужно передать первый кадр - передаются шрифты
    NeedForTransmitSecond, // Это когда нужно передать второй и последующий кадры - шрифты не передаются
    InProcess
};};

static StateTransmit::E stateTransmit = StateTransmit::Free;


void CalculateCurrentColor(void)
{
    if (currentColor == Color::FLASH_10)
    {
        Painter::SetColor(inverseColors ? gColorBack : gColorFill);
    }
    else if (currentColor == Color::FLASH_01)
    {
        Painter::SetColor(inverseColors ? gColorFill : gColorBack);
    }
}


void CalculateColor(uint8 *color)
{
    currentColor = static_cast<Color::E>(*color);
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


void Painter::ResetFlash(void)
{
    Timer_Enable(TypeTimer::FlashDisplay, 400, OnTimerFlashDisplay);
    inverseColors = false;
}


void Painter::DrawRectangle(int x, int y, int width, int height)
{
    Painter::DrawHLine(y, x, x + width);
    Painter::DrawVLine(x, y, y + height);
    Painter::DrawHLine(y + height, x, x + width);
    if (x + width < SCREEN_WIDTH)
    {
        Painter::DrawVLine(x + width, y, y + height);
    }
}


void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
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
            Painter::DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        Painter::DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}


void Painter::SetFont(TypeFont::E)
{
}


void Painter::LoadFont(TypeFont::E)
{
}


void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
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
            Painter::DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        Painter::DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}


void Painter::SendToDisplay(uint8 *, int)
{
}


void Painter::SetPalette(Color::E)
{
}


void Painter::SetColor(Color::E)
{
}


void Painter::DrawHLine(int, int, int)
{
}


void Painter::DrawVLine(int, int, int)
{
}


void Painter::DrawVPointLine(int, int, int, float, Color::E)
{
}


void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += static_cast<int>(delta))
    {
        Painter::SetPoint(x, y);
    }
}


void Painter::SetPoint(int, int )
{
}


void Painter::DrawMultiVPointLine(int , int , uint16 *, int , int , Color::E ) 
{
}


void Painter::DrawMultiHPointLine(int , int , uint8 *, int , int , Color::E )
{
}


void Painter::DrawLine(int x0, int y0, int x1, int y1)
{
    if (x0 == x1)
    {
        Painter::DrawVLine(x0, y0, y1);
    }
    else if (y0 == y1)
    {
        Painter::DrawHLine(y0, x0, x1);
    }
}


void Painter::FillRegion(int , int , int , int )
{
}


void Painter::DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade)
{
    if (inShade)
    {
        thickness = 1;
    }
    Region(width - thickness * 2, height - thickness * 2).Fill(x + thickness, y + thickness, normal);

    if (isPressed || inShade)
    {
        for (int i = 0; i < thickness; i++)
        {
            HLine().Draw(y + i, x + i, x + width - i, dark);
            Painter::DrawVLine(x + i, y + 1 + i, y + height - i);
            VLine().Draw(x + width - i, y + 1 + i, y + height - i, bright);
            Painter::DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            HLine().Draw(y + i, x + i, x + width - i, bright);
            Painter::DrawVLine(x + i, y + 1 + i, y + height - i);
            VLine().Draw(x + width - i, y + 1 + i, y + height - i, dark);
            Painter::DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
}


void Painter::SetBrightnessDisplay(int16)
{
}


int NumberColorsInSceneCol()
{
    return numberColorsUsed;
}


void Painter::DrawVLineArray(int , int , uint8 *, Color::E )
{
}


void Painter::DrawSignal(int , uint8 [281], bool )
{
}


void Painter::LoadPalette(int num)
{
    int min[] = {0, 5, 10};
    int max[] = {4, 9, 15};

    int i = min[num];
    int a = max[num];

    for (; i <= a; i++)
    {
        Painter::SetPalette(static_cast<Color::E>(i));
    }
}


void Painter::BeginScene(Color::E color)
{
    if (stateTransmit == StateTransmit::NeedForTransmitFirst || stateTransmit == StateTransmit::NeedForTransmitSecond)
    {
        bool needForLoadFontsAndPalette = stateTransmit == StateTransmit::NeedForTransmitFirst;
        stateTransmit = StateTransmit::InProcess;
        if(needForLoadFontsAndPalette) 
        {
            Painter::LoadPalette(0);
            Painter::LoadPalette(1);
            Painter::LoadPalette(2);
            Painter::LoadFont(TypeFont::_5);
            Painter::LoadFont(TypeFont::_8);
            Painter::LoadFont(TypeFont::_UGO);
            Painter::LoadFont(TypeFont::_UGO2);
        }
    }

    Region(319, 239).Fill(0, 0, color);
}


void Painter::EndScene()
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
