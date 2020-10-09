#include "defines.h"
#include "DisplayPrimitives.h"
#include "common/Display/Colors.h"
#include "Display/Painter.h"
#include "common/Display/Primitives.h"
#include <cstdio> 


using namespace Primitives;


void ProgressBar_Draw(const ProgressBar *bar)
{
    int x = bar->x;
    int y = bar->y;
    char buffer[100] = {0};
    float passedPercents = bar->fullTime == 0 ? 0 : bar->passedTime / bar->fullTime * 100; //-V2550 //-V550
    std::sprintf(buffer, "Завершено %.1f %%", passedPercents);
    Painter::DrawStringInCenterRectC(x, y - 15, bar->width, bar->height, buffer, COLOR_FILL);
    Rectangle(bar->width, bar->height).Draw(bar->x, bar->y);
    Region(static_cast<int>(bar->width * passedPercents / 100.0F), bar->height).Fill(bar->x, bar->y);
    buffer[0] = 0;
    std::sprintf(buffer, "Осталось %.1f с", static_cast<int>(bar->fullTime - bar->passedTime) / 1000.0F);
    Painter::DrawStringInCenterRect(x, y + bar->height, bar->width, bar->height, buffer);
}
