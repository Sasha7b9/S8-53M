#include "defines.h"
#include "common/Display/Primitives_c.h"
#include "Display/Painter.h"
#include "common/Hardware/Timer_c.h"


using namespace Primitives;


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


void Painter::SetFont(TypeFont::E)
{
}


void Painter::LoadFont(TypeFont::E)
{
}


int NumberColorsInSceneCol()
{
    return numberColorsUsed;
}


void Painter::BeginScene(Color::E color)
{
    if (stateTransmit == StateTransmit::NeedForTransmitFirst || stateTransmit == StateTransmit::NeedForTransmitSecond)
    {
        bool needForLoadFontsAndPalette = stateTransmit == StateTransmit::NeedForTransmitFirst;
        stateTransmit = StateTransmit::InProcess;
        if(needForLoadFontsAndPalette) 
        {
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


void Painter::CalculateCurrentColor()
{

}
