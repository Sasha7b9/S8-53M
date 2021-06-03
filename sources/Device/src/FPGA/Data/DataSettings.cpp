// 2021/03/15 13:29:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/Data/DataSettings.h"


void DataSettings::Fill()
{
    enabled_a = ChA.IsEnabled() ? 1U : 0U;
    enabled_b = ChB.IsEnabled() ? 1U : 0U;
    r_shift_a = (uint)set.chan[Channel::A].rshift;
    r_shift_b = (uint)set.chan[Channel::B].rshift;
    trig_lev_a = (uint)TrigLev::GetA();
    trig_lev_b = (uint)TrigLev::GetB();
    couple_a = set.chan[Channel::A].mode_couple;
    couple_b = set.chan[Channel::B].mode_couple;
    tbase = set.time.base;
    tshift = set.time.shift;
    range[0] = set.chan[Channel::A].range;
    range[1] = set.chan[Channel::B].range;
    peak_det = (uint)PeackDetMode::Get();
    multiplier_a = set.chan[Channel::A].divider;
    multiplier_b = set.chan[Channel::B].divider;
    enum_points = set.memory.enum_points_fpga;
    time = HAL_RTC::GetPackedTime();
}


uint DataSettings::BytesInChannel() const
{
    uint result = PointsInChannel();

    if (peak_det != 0)
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
    return EnumPointsFPGA::ToPoints((EnumPointsFPGA::E)enum_points);
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
    return (peak_det != 0);
}


TBase::E DataSettings::GetTBase() const
{
    return (TBase::E)tbase;
}
