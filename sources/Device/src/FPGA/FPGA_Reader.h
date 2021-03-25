// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FPGA/FPGA_Types.h"


struct DataReading;


struct ReaderFPGA
{
    static void ReadData();

    static void ReadRandomizeMode(DataReading &);

    static void ReadRealMode(DataReading &);

    static void ReadPoint();

    static int addition_shift;                  // Дополнительное смещение. Нужно для правильной расстановки точек
                                                // в режиме рандомизатора

private:

    // Инвертирует данные.
    static void InverseDataIsNecessary(const Channel &ch, uint8 *data);

    static int CalculateShift();

    static void ReadRealModePeakDetOn(DataReading &);

    static void ReadRealModePeakDetOff(DataReading &);

    static uint16 ReadAddressStop();

    static void ReadChannel(DataReading &data, const Channel &ch, uint16 addr_stop);
};
