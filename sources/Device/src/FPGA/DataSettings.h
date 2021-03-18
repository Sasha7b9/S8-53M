#pragma once
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_.h"


struct DataSettings
{
    uint                enabled_a : 1;    // Включён ли канал 0
    uint                enabled_b : 1;    // Включен ли канал 1
    uint                rShiftCh0       : 10;   // Смещение по напряжению
    uint                rShiftCh1       : 10;
    uint                trigLevCh0 : 10;   // Уровень синхронизации
    uint                trigLevCh1 : 10;
    ModeCouple::E       modeCouple0 : 2;    // Режим канала по входу
    ModeCouple::E       modeCouple1 : 2;
    TBase::E            tBase : 5;    // Масштаб по времени
    int16               tShift;                 // Смещение по времени
    Range::E            range[2];               // Масштаб по напряжению обоих каналов.
    uint                peakDet : 2;    // Включен ли пиковый детектор
    Divider::E          multiplier0 : 1;
    Divider::E          multiplier1 : 1;
    ENUM_POINTS_FPGA::E enum_points     : 4;    // enum точек в канале
    PackedTime          time;

    void Fill();

    uint BytesInChannel() const;

    uint PointsInChannel() const;

    bool IsEnabled(const Channel &ch) const;

    bool IsEnabledPeakDet() const;
};
