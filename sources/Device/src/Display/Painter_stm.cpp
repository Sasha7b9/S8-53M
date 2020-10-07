#include "defines.h"
#include "Display/Painter.h"


using namespace Primitives;


extern StateTransmit::E stateTransmit;
extern bool noFonts;


void Painter::BeginScene(Color::E color)
{
    if (stateTransmit == StateTransmit::NeedForTransmitFirst || stateTransmit == StateTransmit::NeedForTransmitSecond)
    {
        bool needForLoadFontsAndPalette = stateTransmit == StateTransmit::NeedForTransmitFirst;
        stateTransmit = StateTransmit::InProcess;
        if (needForLoadFontsAndPalette)
        {
            LoadPalette();
            if (!noFonts)                // Если был запрос на загрузку шрифтов
            {
                LoadFont(TypeFont::_5);
                LoadFont(TypeFont::_8);
                LoadFont(TypeFont::_UGO);
                LoadFont(TypeFont::_UGO2);
            }
        }
    }

    Region(319, 239).Fill(0, 0, color);
}


void Painter::EndScene(bool)
{
}


void Painter::SetColor(Color::E)
{
}


void Primitives::Region::Fill(int , int , Color::E )
{

}


void Primitives::HLine::Draw(int , int , int , Color::E )
{

}


void Primitives::Point::Draw(int , int )
{

}
