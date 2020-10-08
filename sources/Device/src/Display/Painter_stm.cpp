#include "defines.h"
#include "common/Display/Primitives.h"
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
            if (!noFonts)                // ���� ��� ������ �� �������� �������
            {
            }
        }
    }

    Region(319, 239).Fill(0, 0, color);
}


void Painter::EndScene(bool)
{
}
