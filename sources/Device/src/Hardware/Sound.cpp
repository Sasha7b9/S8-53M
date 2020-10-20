#include "defines.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include <cmath>


static uint16 CalculatePeriodForTIM();
static void SetWave();
static void CalculateSine();
static void CalculateMeandr();
static void CalculateTriangle();


static const int POINTS_IN_PERIOD = 10;
static uint8 points[POINTS_IN_PERIOD] = {0};
static float frequency = 0.0F;
static float amplitude = 0.0F;
static TypeWave::E typeWave = TypeWave::Sine;


bool Sound::isBeep = false;


static void Stop(void)
{
    HAL_DAC::StopDMA();
    Sound::isBeep = false;
    SOUND_WARN_IS_BEEP = 0;
}


void Sound_Beep(TypeWave::E typeWave_, float frequency_, float amplitude_, int duration)
{
    if (SOUND_WARN_IS_BEEP)
    {
        return;
    }
    if (!SOUND_ENABLED)
    {
        return;
    }
    if (frequency != frequency_ || amplitude != amplitude_ || typeWave != typeWave_) //-V2550 //-V550
    {
        frequency = frequency_;
        amplitude = amplitude_;
        typeWave = typeWave_;
        
        Stop();
        SetWave();
    }

    Stop();
    
    Sound::isBeep = true;

    HAL_DAC::StartDMA(points, POINTS_IN_PERIOD);

    Timer::Enable(TypeTimer::StopSound, duration, Stop);
}


void Sound::ButtonPress(void)
{
    Sound_Beep(TypeWave::Sine, 2000.0F, 0.5F, 50);
    BUTTON_IS_PRESSED = 1;
}


void Sound::ButtonRelease(void)
{
    if (BUTTON_IS_PRESSED)
    {
        Sound_Beep(TypeWave::Sine, 1000.0F, 0.25F, 50);
        BUTTON_IS_PRESSED = 0;
    }
}


void Sound::GovernorChangedValue(void)
{
    Sound_Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
    BUTTON_IS_PRESSED = 0;
}


void Sound::RegulatorShiftRotate(void)
{
    Sound_Beep(TypeWave::Sine, 1.0F, 0.35F, 3);
    BUTTON_IS_PRESSED = 0;
}


void Sound::RegulatorSwitchRotate(void)
{
    Sound_Beep(TypeWave::Triangle, 2500.0F, 0.5F, 25);
    BUTTON_IS_PRESSED = 0;
}


void Sound::WarnBeepBad(void)
{
    Sound_Beep(TypeWave::Meandr, 250.0F, 1.0F, 500);
    SOUND_WARN_IS_BEEP = 1;
    BUTTON_IS_PRESSED = 0;
}


void Sound::WarnBeepGood(void)
{
    Sound_Beep(TypeWave::Triangle, 1000.0F, 0.5F, 250);
    BUTTON_IS_PRESSED = 0;
}


void SetWave(void)
{
    HAL_TIM7::Config(0, CalculatePeriodForTIM());

    if(typeWave == TypeWave::Sine)
    {
        CalculateSine();
    }
    else if(typeWave == TypeWave::Meandr)
    {
        CalculateMeandr();
    }
    else if(typeWave == TypeWave::Triangle)
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
        float step = 2.0F * 3.1415926F / (POINTS_IN_PERIOD - 1);
        float value = (std::sin(static_cast<float>(i) * step) + 1.0F) / 2.0F;
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
