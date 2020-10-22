#include "defines.h"
#include "common/Display/Painter_c.h"
#include "common/Display/Primitives_c.h"


using namespace Primitives;


void Painter::BeginScene(Color::E color)
{
    Region(319, 239).Fill(0, 0, color);
}


void Painter::EndScene(bool)
{
}
