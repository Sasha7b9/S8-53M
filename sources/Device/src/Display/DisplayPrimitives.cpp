#include "defines.h"
#include "common/Display/Colors_c.h"
#include "common/Display/Primitives_c.h"
#include "common/Display/Text_c.h"
#include "Display/DisplayPrimitives.h"
#include <cstdio> 


using namespace Primitives;


void ProgressBar::Draw()
{
    char buffer[100] = {0};
    float passedPercents = fullTime == 0 ? 0 : passedTime / fullTime * 100; //-V2550 //-V550 //-V2564
    std::sprintf(buffer, "Завершено %.1f %%", passedPercents);
    Text(buffer).DrawInCenterRect(x, y - 15, width, height, Color::FILL);
    Rectangle(width, height).Draw(x, y);
    Region(static_cast<int>(width * passedPercents / 100.0F), height).Fill(x, y); //-V2564
    buffer[0] = 0;
    std::sprintf(buffer, "Осталось %.1f с", static_cast<int>(fullTime - passedTime) / 1000.0F); //-V2564
    Text(buffer).DrawInCenterRect(x, y + height, width, height);
}
