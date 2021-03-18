#pragma once
#include "common/Settings/SettingsTypes_.h"


#define SET_INVERSE(ch)         (set.chan[ch].inverse)              // SettingsChannel.inverse
#define SET_INVERSE_A           (SET_INVERSE(ChA))           // set.chan[ChA].inverse
#define SET_INVERSE_B           (SET_INVERSE(ChB))           // set.chan[ChB].inverse
#define SET_RSHIFT_MATH         (set.math.rShift)                   // SettingsMath.rShift
#define SET_RANGE_MATH          (set.math.range)                    // SettingsMath.range

#define SET_RANGE(ch)           (set.chan[ch].range)                // SettingsChannel.range
#define SET_RANGE_A             (SET_RANGE(ChA))
#define SET_RANGE_B             (SET_RANGE(ChB))

#define SET_RSHIFT(ch)          (set.chan[ch].rShiftRel)            // SettingsChannel.rShiftRel
#define SET_RSHIFT_A            (SET_RSHIFT(ChA))
#define SET_RSHIFT_B            (SET_RSHIFT(ChB))

#define SET_COUPLE(ch)          (set.chan[ch].modeCouple)           // SettingsChannel.modeCouple
#define SET_COUPLE_A            (SET_COUPLE(ChA))
#define SET_COUPLE_B            (SET_COUPLE(ChB))

#define SET_DIVIDER(ch)         (set.chan[ch].divider)              // SettingsChannel.divider
#define SET_DIVIDER_A           SET_DIVIDER(ChA)
#define SET_DIVIDER_B           SET_DIVIDER(ChB)
#define SET_DIVIDER_1(ch)       (SET_DIVIDER(ch) == Divider::_1)
#define SET_DIVIDER_10(ch)      (SET_DIVIDER(ch) == Divider::_10)
#define VALUE_MULTIPLIER(ch)    (Divider::ToAbs(SET_DIVIDER(ch)))

#define SET_ENABLED(ch)         (set.chan[ch].enable)               // SettingsChannel.enable
#define SET_ENABLED_A           SET_ENABLED(ChA)             // set.chan[ChA].enable
#define SET_ENABLED_B           SET_ENABLED(ChB)             // set.chan[ChB].enable

#define SET_BALANCE_ADC(ch)     (set.chan[ch].balanceShiftADC)      // SettingsChannel.balanceShiftADC
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(ChA))       // set.chan[ChA].balanceShiftADC
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(ChB))       // set.chan[ChB].balanceShiftADC

#define STRETCH_ADC(ch)         (set.chan[ch].stretchADC)           // SettingsChannel.stretchADC
#define STRETCH_ADC_A           (STRETCH_ADC(ChA))           // set.chan[ChA].stretchADC
#define STRETCH_ADC_B           (STRETCH_ADC(ChB))           // set.chan[ChB].stretchADC

#define SET_FILTR(ch)           (set.chan[ch].filtr)                // SettingsChannel.filtr
#define SET_FILTR_A             (SET_FILTR(ChA))             // set.chan[ChA].filtr
#define SET_FILTR_B             (SET_FILTR(ChB))             // set.chan[ChB].filtr


// Установить масштаб по напряжению канала chan.
void sChannel_SetRange(Channel::E ch, Range::E range);

// Возвращает true, если канал chan включён.
bool sChannel_Enabled(Channel::E ch);

pchar sChannel_Range2String(Range::E range, Divider::E multiplier);

String sChannel_RShift2String(int16 rShiftRel, Range::E range, Divider::E multiplier);
