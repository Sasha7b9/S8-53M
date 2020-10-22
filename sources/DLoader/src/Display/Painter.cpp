#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "Display/Painter.h"
#include "common/Hardware/Timer_c.h"


using namespace Primitives;


static bool inverseColors = false;
static int numberColorsUsed = 0;


struct StateTransmit { enum E
{
    Free,
    NeedForTransmitFirst,  // Это когда нужно передать первый кадр - передаются шрифты
    NeedForTransmitSecond, // Это когда нужно передать второй и последующий кадры - шрифты не передаются
    InProcess
};};

static StateTransmit::E stateTransmit = StateTransmit::Free;


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
    Timer::Enable(TypeTimer::FlashDisplay, 400, OnTimerFlashDisplay);
    inverseColors = false;
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


void Painter::SetColor(Color::E)
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


void Painter::FillRegion(int , int , int , int )
{
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


void Painter::DrawCharHardCol(int , int , char )
{

}


void Painter::CalculateCurrentColor()
{

}
