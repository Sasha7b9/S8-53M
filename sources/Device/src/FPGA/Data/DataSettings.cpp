// 2021/03/15 13:29:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/Data/DataSettings.h"
#include "Settings/Settings.h"


void DataSettings::Fill()
{
    enabled_a = ChA.IsEnabled() ? 1U : 0U;
    enabled_b = ChB.IsEnabled() ? 1U : 0U;
    r_shift_a = (uint)RShift::Get(ChA);
    r_shift_b = (uint)RShift::Get(ChB);
    trig_lev_a = (uint)TrigLev::GetA();
    trig_lev_b = (uint)TrigLev::GetB();
    couple_a = set.chan[ChA].mode_ñouple;
    couple_b = set.chan[ChB].mode_ñouple;
    tBase = TBase::Get();
    tShift = TShift::Get();
    range[0] = set.chan[ChA].range;
    range[1] = set.chan[ChB].range;
    peakDet = (uint)PeackDetMode::Get();
    multiplier0 = set.chan[ChA].divider;
    multiplier1 = set.chan[ChB].divider;
    enum_points = set.memory.enum_points_fpga;
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
