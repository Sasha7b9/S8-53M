#include "defines.h"
#include "Sound.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Log.h"
#include "Settings/Settings.h"
#include <cmath>
#include "globals.h"



static uint16 CalculatePeriodForTIM();
static void SetWave();
static void CalculateSine();
static void CalculateMeandr();
static void CalculateTriangle();


static const int POINTS_IN_PERIOD = 10;
static uint8 points[POINTS_IN_PERIOD] = {0};
static float frequency = 0.0f;
static float amplitude = 0.0f;
static TypeWave typeWave = TypeWave_Sine;



static void Stop(void)
{
    HAL_DAC::StopDMA();
    SOUND_IS_BEEP = 0;
    SOUND_WARN_IS_BEEP = 0;
}


void Sound_Beep(TypeWave typeWave_, float frequency_, float amplitude_, int duration)
{
    if (SOUND_WARN_IS_BEEP)
    {
        return;
    }
    if (!SOUND_ENABLED)
    {
        return;
    }
    if (frequency != frequency_ || amplitude != amplitude_ || typeWave != typeWave_)
    {
        frequency = frequency_;
        amplitude = amplitude_;
        typeWave = typeWave_;
        
        Stop();
        SetWave();
    }

    Stop();
    
    SOUND_IS_BEEP = 1;

    HAL_DAC::StartDMA(points, POINTS_IN_PERIOD);

    Timer::Enable(kStopSound, duration, Stop);
}


void Sound::ButtonPress(void)
{
    Sound_Beep(TypeWave_Sine, 2000.0f, 0.5f, 50);
    BUTTON_IS_PRESSED = 1;
}


void Sound::ButtonRelease(void)
{
    if (BUTTON_IS_PRESSED)
    {
        Sound_Beep(TypeWave_Sine, 1000.0f, 0.25f, 50);
        BUTTON_IS_PRESSED = 0;
    }
}


void Sound::GovernorChangedValue(void)
{
    Sound_Beep(TypeWave_Sine, 1000.0f, 0.5f, 50);
    BUTTON_IS_PRESSED = 0;
}


void Sound::RegulatorShiftRotate(void)
{
    Sound_Beep(TypeWave_Sine, 1.0f, 0.35f, 3);
    BUTTON_IS_PRESSED = 0;
}


void Sound::RegulatorSwitchRotate(void)
{
    Sound_Beep(TypeWave_Triangle, 2500.0f, 0.5f, 25);
    BUTTON_IS_PRESSED = 0;
}


void Sound::WarnBeepBad(void)
{
    Sound_Beep(TypeWave_Meandr, 250.0f, 1.0f, 500);
    SOUND_WARN_IS_BEEP = 1;
    BUTTON_IS_PRESSED = 0;
}


void Sound::WarnBeepGood(void)
{
    Sound_Beep(TypeWave_Triangle, 1000.0f, 0.5f, 250);
    BUTTON_IS_PRESSED = 0;
}


void SetWave(void)
{
    HAL_TIM7::Config(0, CalculatePeriodForTIM());

    if(typeWave == TypeWave_Sine)
    {
        CalculateSine();
    }
    else if(typeWave == TypeWave_Meandr)
    {
        CalculateMeandr();
    }
    else if(typeWave == TypeWave_Triangle)
    {
        CalculateTriangle();
    }
}


uint16 CalculatePeriodForTIM(void)
{
    return static_cast<uint16>(120e6F / frequency / POINTS_IN_PERIOD);
}


void CalculateSine(void)
{
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        float step = 2.0f * 3.1415926f / (POINTS_IN_PERIOD - 1);
        float value = (std::sin(static_cast<float>(i) * step) + 1.0f) / 2.0f;
        points[i] = static_cast<uint8>(value * amplitude * 255);
    }
}


void CalculateMeandr(void)
{
    for (int i = 0; i < POINTS_IN_PERIOD / 2; i++)
    {
        points[i] = static_cast<uint8>(255 * amplitude);
    }
    for (int i = POINTS_IN_PERIOD / 2; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = 0;
    }
}


void CalculateTriangle(void)
{
    float k = 255.0 / POINTS_IN_PERIOD;
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = static_cast<uint8>(k * i * amplitude);
    }
}
