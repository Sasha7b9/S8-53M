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
    NeedForTransmitFirst,  // ��� ����� ����� �������� ������ ���� - ���������� ������
    NeedForTransmitSecond, // ��� ����� ����� �������� ������ � ����������� ����� - ������ �� ����������
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


void Painter::SetPoint(int, int )
{
}


void Painter::DrawMultiVPointLine(int , int , uint16 *, int , int , Color::E ) 
{
}


void Painter::DrawMultiHPointLine(int , int , uint8 *, int , int , Color::E )
{
}


void Painter::SetBrightnessDisplay(int16)
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


void Painter::DrawCharHardCol(int , int , char )
{

}


void Painter::CalculateCurrentColor()
{

}
