#pragma once
#include "Settings/SettingsChannel.h"
#include "Settings/SettingsTime.h"
#include "common/Hardware/HAL/HAL_.h"


struct DataSettings
{
    void*               addrNext;               // Адрес следующей записи.
    void*               addrPrev;               // Адрес предыдущей записи.
    uint                rShiftCh0       : 10;   // Смещение по напряжению
    uint                rShiftCh1       : 10;
    uint                trigLevCh0      : 10;   // Уровень синхронизации
    int16               tShift;                 // Смещение по времени
    ModeCouple::E       modeCouple1     : 2;
    Range::E            range[2];               // Масштаб по напряжению обоих каналов.
    uint                trigLevCh1      : 10;
    uint                length1channel  : 11;   // Сколько занимает в байтах длина измерения одного канала
    TBase::E            tBase           : 5;    // Масштаб по времени
    ENUM_POINTS_FPGA::E enum_points     : 4;    // enum точек в канале
    ModeCouple::E       modeCouple0     : 2;    // Режим канала по входу
    uint                peakDet         : 2;    // Включен ли пиковый детектор
    uint                enabled_a       : 1;    // Включён ли канал 0
    uint                enabled_b       : 1;    // Включен ли канал 1
    uint                inverseCh0      : 1;
    uint                inverseCh1      : 1;
    Divider::E          multiplier0     : 1;
    Divider::E          multiplier1     : 1;
    PackedTime          time;

    void Fill();

    int BytesInChannel() const;

    int PointsInChannel() const;

    bool IsEnabled(const Channel &ch) const;
};
