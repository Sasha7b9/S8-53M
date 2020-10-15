#pragma once
#include "common/Settings/SettingsTypes_c.h"
#include "SettingsTypes.h"


#define SET_INVERSE(ch)         (set.chan[ch].inverse)              // SettingsChannel.inverse
#define SET_INVERSE_A           (SET_INVERSE(Channel::A))           // set.chan[Channel::A].inverse
#define SET_INVERSE_B           (SET_INVERSE(Channel::B))           // set.chan[Channel::B].inverse
#define SET_RSHIFT_MATH         (set.math.rShift)                   // SettingsMath.rShift
#define SET_RANGE_MATH          (set.math.range)                    // SettingsMath.range

#define SET_RANGE(ch)           (set.chan[ch].range)                // SettingsChannel.range
#define SET_RANGE_A             (SET_RANGE(Channel::A))
#define SET_RANGE_B             (SET_RANGE(Channel::B))

#define SET_RSHIFT(ch)          (set.chan[ch].rShiftRel)            // SettingsChannel.rShiftRel
#define SET_RSHIFT_A            (SET_RSHIFT(Channel::A))
#define SET_RSHIFT_B            (SET_RSHIFT(Channel::B))

#define SET_COUPLE(ch)          (set.chan[ch].modeCouple)           // SettingsChannel.modeCouple
#define SET_COUPLE_A            (SET_COUPLE(Channel::A))
#define SET_COUPLE_B            (SET_COUPLE(Channel::B))

#define SET_DIVIDER(ch)         (set.chan[ch].divider)              // SettingsChannel.divider
#define SET_DIVIDER_A           SET_DIVIDER(Channel::A)
#define SET_DIVIDER_B           SET_DIVIDER(Channel::B)
#define SET_DIVIDER_1(ch)       (SET_DIVIDER(ch) == Divider::_1)
#define SET_DIVIDER_10(ch)      (SET_DIVIDER(ch) == Divider::_10)
#define VALUE_MULTIPLIER(ch)    (Divider::ToAbs(SET_DIVIDER(ch)))

#define SET_ENABLED(ch)         (set.chan[ch].enable)               // SettingsChannel.enable
#define SET_ENABLED_A           SET_ENABLED(Channel::A)             // set.chan[Channel::A].enable
#define SET_ENABLED_B           SET_ENABLED(Channel::B)             // set.chan[Channel::B].enable

#define SET_BALANCE_ADC(ch)     (set.chan[ch].balanceShiftADC)      // SettingsChannel.balanceShiftADC
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(Channel::A))       // set.chan[Channel::A].balanceShiftADC
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(Channel::B))       // set.chan[Channel::B].balanceShiftADC

#define STRETCH_ADC(ch)         (set.chan[ch].stretchADC)           // SettingsChannel.stretchADC
#define STRETCH_ADC_A           (STRETCH_ADC(Channel::A))           // set.chan[Channel::A].stretchADC
#define STRETCH_ADC_B           (STRETCH_ADC(Channel::B))           // set.chan[Channel::B].stretchADC

#define SET_FILTR(ch)           (set.chan[ch].filtr)                // SettingsChannel.filtr
#define SET_FILTR_A             (SET_FILTR(Channel::A))             // set.chan[Channel::A].filtr
#define SET_FILTR_B             (SET_FILTR(Channel::B))             // set.chan[Channel::B].filtr


// Установить масштаб по напряжению канала chan.
void sChannel_SetRange(Channel::E chan, Range::E range);

// Возвращает true, если канал chan включён.
bool sChannel_Enabled(Channel::E chan);

const char *sChannel_Range2String(Range::E range, Divider::E multiplier);

const char *sChannel_RShift2String(int16 rShiftRel, Range::E range, Divider::E multiplier, char buffer[20]);
