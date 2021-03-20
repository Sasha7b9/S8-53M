#pragma once
#include "common/Settings/SettingsTypes_.h"


#define SET_INVERSE(ch)         (set.chan[ch].inverse)              // SettingsChannel.inverse
#define SET_INVERSE_A           (SET_INVERSE(ChA))                  // set.chan[ChA].inverse
#define SET_INVERSE_B           (SET_INVERSE(ChB))                  // set.chan[ChB].inverse
#define SET_RSHIFT_MATH         (set.math.rShift)                   // SettingsMath.rShift
#define SET_RANGE_MATH          (set.math.range)                    // SettingsMath.range

#define VALUE_MULTIPLIER(ch)    (Divider::ToAbs(Divider::Get(ch)))

#define SET_ENABLED(ch)         (set.chan[ch].enable)               // SettingsChannel.enable
#define SET_ENABLED_A           SET_ENABLED(ChA)                    // set.chan[ChA].enable
#define SET_ENABLED_B           SET_ENABLED(ChB)                    // set.chan[ChB].enable

#define SET_BALANCE_ADC(ch)     (set.chan[ch].balanceShiftADC)      // SettingsChannel.balanceShiftADC
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(ChA))              // set.chan[ChA].balanceShiftADC
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(ChB))              // set.chan[ChB].balanceShiftADC
