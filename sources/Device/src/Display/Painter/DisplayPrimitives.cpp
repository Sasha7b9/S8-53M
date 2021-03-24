#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Display/Painter/DisplayPrimitives.h"
#include <cstdio> 


void ProgressBar::Draw()
{
    char buffer[100] = {0};
    float passedPercents = fullTime == 0 ? 0 : passedTime / fullTime * 100;
    std::sprintf(buffer, "Завершено %.1f %%", passedPercents);
    Text(buffer).DrawInCenterRect(x, y - 15, width, height, Color::FILL);
    Rectangle(width, height).Draw(x, y);
    Region((int)(width * passedPercents / 100.0F), height).Fill(x, y);
    buffer[0] = 0;
    std::sprintf(buffer, "Осталось %.1f с", (int)(fullTime - passedTime) / 1000.0F);
    Text(buffer).DrawInCenterRect(x, y + height, width, height);
}
