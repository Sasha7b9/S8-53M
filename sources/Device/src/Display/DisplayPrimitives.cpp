#include "defines.h"
#include "DisplayPrimitives.h"
#include "common/Display/Colors.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include <cstdio> 


using namespace Primitives;


void ProgressBar::Draw()
{
    char buffer[100] = {0};
    float passedPercents = fullTime == 0 ? 0 : passedTime / fullTime * 100; //-V2550 //-V550
    std::sprintf(buffer, "Завершено %.1f %%", passedPercents);
    Painter::DrawStringInCenterRectC(x, y - 15, width, height, buffer, COLOR_FILL);
    Rectangle(width, height).Draw(x, y);
    Region(static_cast<int>(width * passedPercents / 100.0F), height).Fill(x, y);
    buffer[0] = 0;
    std::sprintf(buffer, "Осталось %.1f с", static_cast<int>(fullTime - passedTime) / 1000.0F);
    Painter::DrawStringInCenterRect(x, y + height, width, height, buffer);
}
