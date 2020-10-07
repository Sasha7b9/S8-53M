#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"


using namespace Primitives;


extern StateTransmit::E stateTransmit;
extern bool noFonts;


/// Нарисовать одну вертикальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawVPointLine(int x, int y, int count, int delta);


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


void Primitives::VLine::Draw(int , int , int , Color::E)
{

}


Primitives::MultiVPointLine::MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) : numLines(_numLines), x0(_x0), delta(_delta), count(_count)
{

}


void Primitives::MultiVPointLine::Draw(int y, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


static void DrawVPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        Point().Draw(x, y);
        y += delta;
    }
}


void Primitives::Rectangle::Draw(int x, int y, Color::E color)
{
    Painter::SetColor(color);

    HLine().Draw(y, x, x + width);
    VLine().Draw(x, y, y + height);
    HLine().Draw(y + height, x, x + width);
    if (x + width < Display::WIDTH)
    {
        HLine().Draw(x + width, y, y + height);
    }
}
