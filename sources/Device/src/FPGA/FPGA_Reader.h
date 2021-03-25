// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"
#include "FPGA/FPGA_Types.h"


struct DataReading;


struct ReaderFPGA
{
    static void ReadData();

    static void ReadPoint();

    static int addition_shift;                  // �������������� ��������. ����� ��� ���������� ����������� �����
                                                // � ������ �������������

    static Mutex mutex_read;                    // ������� �� ������ ������

private:

    // ����������� ������.
    static void InverseDataIsNecessary(const Channel &ch, uint8 *data);

    static int CalculateShift();

    static uint16 ReadAddressStop();

    struct Read
    {
        static void PeakDetOn(DataReading &);

        static void PeakDetOff(DataReading &);

        struct Real
        {
            static void Channel(DataReading &data, const Channel &ch, uint16 addr_stop);
        };

        struct Randomizer
        {
            static void Channel(DataReading &data, const Channel &ch, uint16 addr_stop);
        };
    };
};
