#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"


struct DataSettings
{
    DataSettings() : enabled_a(0), enabled_b(0), is_clean(1) {} //-V730

    uint                enabled_a    : 1;   // Включён ли канал 0
    uint                enabled_b    : 1;   // Включен ли канал 1
    uint                r_shift_a    : 10;  // Смещение по напряжению
    uint                r_shift_b    : 10;
    uint                trig_lev_a   : 10;  // Уровень синхронизации
    uint                trig_lev_b   : 10;
    uint                couple_a     : 2;   // Режим канала по входу
    uint                couple_b     : 2;
    uint                tbase        : 5;   // Масштаб по времени
    uint                peak_det     : 2;   // Включен ли пиковый детектор
    Divider::E          multiplier_a : 1;
    Divider::E          multiplier_b : 1;
    EnumPointsFPGA::E enum_points  : 4;   // enum точек в канале
    uint                is_clean     : 1;   // установленное в 1 значение означает, что данных нет (структура только
                                            // что создана)
    PackedTime          time;
    int16               tshift;             // Смещение по времени
    Range::E            range[2];           // Масштаб по напряжению обоих каналов.


    void Fill();

    TBase::E GetTBase() const;

    // Возвращает количество байт, занимаемых одним каналом
    uint BytesInChannel() const;

    // Возвращает количество байт, занимаемых обоими каналами (с учётом их включения)
    uint BytesInData() const;

    // Сколько точек в одном канале
    uint PointsInChannel() const;

    bool IsEnabled(const Channel &ch) const;

    bool IsEnabledPeakDet() const;
};
