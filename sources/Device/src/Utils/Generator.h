#pragma once
#include "Settings/SettingsTypes.h"


struct TypeWave { enum E
{
    Sinus,
    Meander
};};

struct SGenerator
{
    void(*SetParametersWave)(Channel::E channel, TypeWave::E typeWave, float frequency, float startAngle, float amplWave, float amplNoise);    // numWave = 1, 2. ����� ���� �� ���� ��������.
                                                                                                    // amplWave = 0.0F...1.0F, amplNoise = 0.0F...1.0F - ������������ ��������� �������.
    void(*StartNewWave)(Channel::E channel);
    uint8(*GetSampleWave)(Channel::E channel);
};

extern const SGenerator Generator;
