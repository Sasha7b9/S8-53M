#pragma once


#include "defines.h"
#include "Settings/SettingsTypes.h"


enum TypeWave
{
    Wave_Sinus,
    Wave_Meander
};

struct SGenerator
{
    void(*SetParametersWave)(Channel channel, TypeWave typeWave, float frequency, float startAngle, float amplWave, float amplNoise);    // numWave = 1, 2. Может быть до двух сигналов.
                                                                                                    // amplWave = 0.0F...1.0f, amplNoise = 0.0F...1.0f - относительно амплитуды сигнала.
    void(*StartNewWave)(Channel channel);
    uint8(*GetSampleWave)(Channel channel);
};

extern const SGenerator Generator;
