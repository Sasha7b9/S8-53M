#pragma once
#include "common/Settings/SettingsTypes_.h"


#define SET_INVERSE(ch)         (set.chan[ch].inverse)              // SettingsChannel.inverse
#define SET_INVERSE_A           (SET_INVERSE(ChA))                  // set.chan[ChA].inverse
#define SET_INVERSE_B           (SET_INVERSE(ChB))                  // set.chan[ChB].inverse
#define SET_RSHIFT_MATH         (set.math.rShift)                   // SettingsMath.rShift
#define SET_RANGE_MATH          (set.math.range)                    // SettingsMath.range

#define VALUE_MULTIPLIER(ch)    (Divider::ToAbs(Divider::Get(ch)))
