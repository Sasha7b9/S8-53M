#include "defines.h"
#include "Display/Painter.h"


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

    FillRegionC(0, 0, 319, 239, color);
}
