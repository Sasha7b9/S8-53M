// 2021/03/15 13:29:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "Settings/Settings.h"


void DataSettings::Fill()
{
    enabled_a = ChA.IsEnabled() ? 1U : 0U;
    enabled_b = ChB.IsEnabled() ? 1U : 0U;
    r_shift_a = (uint)SET_RSHIFT_A;
    rShiftCh1 = (uint)SET_RSHIFT_B;
    trigLevCh0 = (uint)TRIG_LEVEL_A;
    trigLevCh1 = (uint)TRIG_LEVEL_B;
    modeCouple0 = SET_COUPLE_A;
    modeCouple1 = SET_COUPLE_B;
    tBase = SET_TBASE;
    tShift = TSHIFT;
    range[0] = SET_RANGE_A;
    range[1] = SET_RANGE_B;
    peakDet = (uint)PEAKDET;
    multiplier0 = SET_DIVIDER_A;
    multiplier1 = SET_DIVIDER_B;
    enum_points = ENUM_POINTS;
    time = HAL_RTC::GetPackedTime();
}


uint DataSettings::BytesInChannel() const
{
    uint result = PointsInChannel();

    if (peakDet != 0)
    {
        result *= 2;
    }

    return result;
}


uint DataSettings::BytesInData() const
{
    uint result = 0;

    uint bytes_in_channel = BytesInChannel();

    if (IsEnabled(Channel::A))
    {
        result += bytes_in_channel;
    }

    if (IsEnabled(Channel::B))
    {
        result += bytes_in_channel;
    }

    return result;
}


uint DataSettings::PointsInChannel() const
{
    return ENUM_POINTS_FPGA::ToPoints((ENUM_POINTS_FPGA::E)enum_points);
}


bool DataSettings::IsEnabled(const Channel &ch) const
{
    if (ch.IsA())
    {
        return (enabled_a != 0);
    }

    return (enabled_b != 0);
}


bool DataSettings::IsEnabledPeakDet() const
{
    return (peakDet != 0);
}
