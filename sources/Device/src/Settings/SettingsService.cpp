#include "Settings.h"
#include "SettingsService.h"



bool sService_MathEnabled()
{
    return !DISABLED_DRAW_MATH || ENABLED_FFT;
}


float sService_MathGetFFTmaxDBabs()
{
    static const float db[] = {-40.0F, -60.0F, -80.0F};
    return db[FFT_MAX_DB];
}
