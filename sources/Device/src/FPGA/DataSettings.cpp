// 2021/03/15 13:29:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/DataSettings.h"
#include "Settings/Settings.h"


void DataSettings::Fill()
{
    enabled_a = ChA.IsEnabled() ? 1U : 0U;
    enabled_b = ChB.IsEnabled() ? 1U : 0U;
    r_shift_a = (uint)RShift::Get(ChA);
    rShiftCh1 = (uint)RShift::Get(ChB);
    trigLevCh0 = (uint)TrigLev::GetA();
    trigLevCh1 = (uint)TrigLev::GetB();
    modeCouple0 = ModeCouple::GetA();
    modeCouple1 = ModeCouple::GetB();
    tBase = TBase::Get();
    tShift = TShift::Get();
    range[0] = Range::GetA();
    range[1] = Range::GetB();
    peakDet = (uint)PeackDetMode::Get();
    multiplier0 = Divider::GetA();
    multiplier1 = Divider::GetB();
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
