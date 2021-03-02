#include "defines.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Hardware/HAL/HAL_.h"


using namespace Primitives;


void Painter::BeginScene(const Color &color)
{
    Region(319, 239).Fill(0, 0, color);
}


void Painter::EndScene(bool)
{
    HAL_LTDC::ToggleBuffers();
}
