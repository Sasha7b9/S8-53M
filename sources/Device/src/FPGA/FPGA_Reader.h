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
    static void InverseDataIsNecessary(DataReading &data, const Channel &ch);

    static Int CalculateShift();

    // ���������� �����, � �������� ����� ������ ������ �����
    static uint16 CalculateAddressRead();

    struct Read
    {
        struct Real
        {
            static void Channel(DataReading &data, const Channel &ch, uint16 addr_read);
        };

        struct Randomizer
        {
            static void Channel(DataReading &data, const Channel &ch, uint16 addr_read);

            // ������������ ������ ������ ����������� ����� � ������� � ����� ���������� �����, ������� ����� ���������
            // ��� ������
            static bool IndexFirstPoint(int *first_out, int *skipped_out);

            // ������� � �������� num_words ������ �� ������ address
            static void UtilizeFirstBytes(const uint16 * const address, int num_words);
        };
    };
};
