// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"
#include "FPGA/FPGA_Types.h"


struct DataReading;


struct ReaderFPGA
{
    static void ReadData();

    static void ReadPoint();

    static int addition_shift;                  // Дополнительное смещение. Нужно для правильной расстановки точек
                                                // в режиме рандомизатора

    static Mutex mutex_read;                    // Мьютекс на чтение данных

private:

    // Инвертирует данные.
    static void InverseDataIsNecessary(const Channel &ch, uint8 *data);

    static int CalculateShift();

    static void ReadPeakDetOn(DataReading &);

    static void ReadPeakDetOff(DataReading &);

    static uint16 ReadAddressStop();

    static void ReadChannelPeakDetOff(DataReading &data, const Channel &ch, uint16 addr_stop);

    struct Read
    {
        static void PeakDetOn(DataReading &);

        static void PeakDetOff(DataReading &);
    };
};
