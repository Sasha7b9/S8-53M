// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Timer_.h"
#include "Settings/SettingsCommon.h"
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


volatile bool Sound::isBeep = false;
bool Sound::warnIsBeep = false;
bool Sound::buttonIsPressed = false;


static void Stop()
{
    HAL_DAC2::StopDMA();
    Sound::isBeep = false;
    Sound::warnIsBeep = false;
}


void Sound_Beep(TypeWave::E typeWave_, float frequency_, float amplitude_, int duration)
{
    if (Sound::warnIsBeep)
    {
        return;
    }

    if (!set.service.sound_enabled)
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
    
    Sound::isBeep = true;

    HAL_DAC2::StartDMA(points, POINTS_IN_PERIOD);

    Timer::Enable(TypeTimer::StopSound, duration, Stop);
}


void Sound::ButtonPress()
{
    Sound_Beep(TypeWave::Sine, 2000.0F, 0.5F, 50);
    Sound::buttonIsPressed = true;
}


void Sound::ButtonRelease()
{
    if (buttonIsPressed)
    {
        Sound_Beep(TypeWave::Sine, 1000.0F, 0.25F, 50);
        buttonIsPressed = false;
    }
}


void Sound::GovernorChangedValue()
{
    Sound_Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
    buttonIsPressed = false;
}


void Sound::RegulatorShiftRotate()
{
    Sound_Beep(TypeWave::Sine, 1.0F, 0.35F, 3);
    buttonIsPressed = false;
}


void Sound::RegulatorSwitchRotate()
{
    Sound_Beep(TypeWave::Triangle, 2500.0F, 0.5F, 25);
    buttonIsPressed = false;
}


void Sound::WarnBeepBad()
{
    Sound_Beep(TypeWave::Meandr, 250.0F, 1.0F, 500);
    warnIsBeep = true;
    buttonIsPressed = false;
}


void Sound::WarnBeepGood()
{
    Sound_Beep(TypeWave::Triangle, 1000.0F, 0.5F, 250);
    buttonIsPressed = false;
}


void SetWave()
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


uint16 CalculatePeriodForTIM()
{
    return (uint16)(120e6F / frequency / POINTS_IN_PERIOD);
}


void CalculateSine()
{
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        float step = 2.0F * 3.1415926F / (POINTS_IN_PERIOD - 1);
        float value = (std::sin((float)(i) * step) + 1.0F) / 2.0F;
        points[i] = (uint8)(value * amplitude * 255);
    }
}


void CalculateMeandr()
{
    for (int i = 0; i < POINTS_IN_PERIOD / 2; i++)
    {
        points[i] = (uint8)(255 * amplitude);
    }
    for (int i = POINTS_IN_PERIOD / 2; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = 0;
    }
}


void CalculateTriangle()
{
    float k = 255.0 / POINTS_IN_PERIOD;
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = (uint8)(k * i * amplitude);
    }
}


void Sound::WaitForCompletion()
{

}
