#pragma once


#include "defines.h"
#include "Settings/SettingsTypes.h"


struct TypeWave { enum E
{
    Sinus,
    Meander
};};

struct SGenerator
{
    void(*SetParametersWave)(Channel::E channel, TypeWave::E typeWave, float frequency, float startAngle, float amplWave, float amplNoise);    // numWave = 1, 2. Может быть до двух сигналов.
                                                                                                    // amplWave = 0.0F...1.0f, amplNoise = 0.0F...1.0f - относительно амплитуды сигнала.
    void(*StartNewWave)(Channel::E channel);
    uint8(*GetSampleWave)(Channel::E channel);
};

extern const SGenerator Generator;
