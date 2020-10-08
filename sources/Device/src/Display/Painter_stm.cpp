#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"


using namespace Primitives;


extern StateTransmit::E stateTransmit;
extern bool noFonts;


// Ќарисовать одну вертикальную лиинию из count точек c рассто€нием delta между соседнимит точками
static void DrawVPointLine(int x, int y, int count, int delta);
// Ќарисовать одну горизонтальную лиинию из count точек c рассто€нием delta между соседнимит точками
static void DrawHPointLine(int x, int y, int count, int delta);


void Painter::BeginScene(Color::E color)
{
    if (stateTransmit == StateTransmit::NeedForTransmitFirst || stateTransmit == StateTransmit::NeedForTransmitSecond)
    {
        bool needForLoadFontsAndPalette = stateTransmit == StateTransmit::NeedForTransmitFirst;
        stateTransmit = StateTransmit::InProcess;
        if (needForLoadFontsAndPalette)
        {
            LoadPalette();
            if (!noFonts)                // ≈сли был запрос на загрузку шрифтов
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


void Primitives::MultiHPointLine::Draw(int x, Color::E color)
{
    Painter::SetColor(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


static void DrawHPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        Point().Draw(x, y);
        x += delta;
    }
}
