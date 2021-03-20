#pragma once
#include "common/Settings/SettingsTypes_.h"


#define SET_INVERSE(ch)         (set.chan[ch].inverse)              // SettingsChannel.inverse
#define SET_INVERSE_A           (SET_INVERSE(ChA))                  // set.chan[ChA].inverse
#define SET_INVERSE_B           (SET_INVERSE(ChB))                  // set.chan[ChB].inverse
