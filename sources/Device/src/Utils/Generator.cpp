#include "defines.h"
#include "common/Utils/Math_c.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include "Utils/Generator.h"
#include <cmath>


static void SetParametersWave(Channel::E channel, TypeWave::E typeWave, float frequency, float startAngle, float amplWave, float amplNoise);
static void StartNewWave(Channel::E channel);
static uint8 GetSampleWave(Channel::E channel);


const SGenerator Generator = 
{
    SetParametersWave,
    StartNewWave,
    GetSampleWave,
};


static float NewNoiseValue(Channel::E channel);
static uint8 GetSampleSinusWave(Channel::E channel, int numSample);


static TypeWave::E type[2] = {TypeWave::Sinus, TypeWave::Meander};
static float freq[2] = {1000.0F, 500.0F};
static float angle[2] = {0.05F, 0.1F};
static float ampl[2] = {1.0F, 0.5F};
static float amplNoise[2] = {0.1F, 0.1F};
static int numSample[2] = {0, 0};

void SetParametersWave(Channel::E channel, TypeWave::E typeWave, float frequency, float startAngle, float amplWave, float amplNoise_)
{
    type[channel] = typeWave;
    freq[channel] = frequency;
    angle[channel] = startAngle;
    ampl[channel] = amplWave;
    amplNoise[channel] = amplNoise_;
}

void StartNewWave(Channel::E)
{
    numSample[0] = 0;
    numSample[1] = 0;
}

uint8 GetSampleWave(Channel::E channel)
{
    return (type[channel] == TypeWave::Sinus) ? GetSampleSinusWave(channel, (numSample[channel])++) : 0U;
}

uint8 GetSampleSinusWave(Channel::E channel, int numSample_)
{
    float dT = numSample_ * TSHIFT_2_ABS(1, SET_TBASE);
    float voltage = ampl[channel] * std::sin(2 * M_PI * freq[channel] * dT + angle[channel]) + NewNoiseValue(channel);
    return Math::VoltageToPoint(voltage, SET_RANGE(channel), SET_RSHIFT(channel));
}

float NewNoiseValue(Channel::E channel)
{
    static float prevNoise[2] = {0.0F, 0.0F};            // Здесь хранятся значения шума из предыдущих точек, необходимые для расчёта шума в текущей точке.

    float noise = prevNoise[channel];

    float halfAmplNoiseAbs = ampl[channel] * amplNoise[channel] / 2.0F;

    float deltaRand = halfAmplNoiseAbs;

    noise += Math::RandFloat(-deltaRand, deltaRand);

    while (noise < -halfAmplNoiseAbs)
    {
        noise += Math::RandFloat(0, deltaRand * 2);
    }

    while (noise > halfAmplNoiseAbs)
    {
        noise -= Math::RandFloat(0, deltaRand * 2);
    }

    prevNoise[channel] = noise;

    return noise;
}
