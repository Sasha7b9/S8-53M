#pragma once
#include "common/Display/Colors_c.h"
#include "DisplayTypes.h"
#include "PainterText.h"


namespace Painter
{
    void BeginScene(Color::E color);

    void CalculateCurrentColor();

    void EndScene(void);
};
