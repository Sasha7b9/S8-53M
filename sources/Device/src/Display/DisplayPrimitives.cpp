#include "DisplayPrimitives.h"
#include "Colors.h"
#include "Painter.h"
#include <cstdio> 


void ProgressBar_Draw(ProgressBar *bar)
{
    int x = bar->x;
    int y = bar->y;
    char buffer[100] = {0};
    float passedPercents = bar->fullTime == 0 ? 0 : bar->passedTime / bar->fullTime * 100;
    std::sprintf(buffer, "Завершено %.1f %%", passedPercents);
    Painter::DrawStringInCenterRectC(x, y - 15, bar->width, bar->height, buffer, COLOR_FILL);
    Painter::DrawRectangle(bar->x, bar->y, bar->width, bar->height);
    Painter::FillRegion(bar->x, bar->y, static_cast<int>(bar->width * passedPercents / 100.0F), bar->height);
    buffer[0] = 0;
    std::sprintf(buffer, "Осталось %.1f с", (int)(bar->fullTime - bar->passedTime) / 1000.0F);
    Painter::DrawStringInCenterRect(x, y + bar->height, bar->width, bar->height, buffer);
}
