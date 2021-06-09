// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"
#include "FPGA/FPGA_Types.h"


struct DataReading;


struct ReaderFPGA
{
    static void ReadData();

    static void ReadPoint();

    // ���������� �����, � �������� ����� ������ ������ �����
    static uint16 CalculateAddressRead();

    static Mutex mutex_read;                    // ������� �� ������ ������

    struct ADC
    {
        // ������ ��� ����� (�� ���� ���) � ������ ����������
        static uint16 ReadPoints();

        // ������ ������ ����� �� first �� last
        static void ReadPoints(const Channel &ch, void *first, const void *last);

    private:

        static const uint16 *address;       // � ����� ������ ������������ ������
        static int16  balance;              // ������ ������� ���
        static float  stretch;              // �������� �������� ������

        static uint8 StretchOut(uint8 value);   // ��������� ��������� �� ��� ��������
    };

private:

    // ����������� ������.
    static void InverseDataIsNecessary(DataReading &data, const Channel &ch);

    static Int CalculateShift();

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
            // firts_out - � ���� ����� ����� ��������� ������ ��������� �����
            // skipped_out - ����� ������� ����� ����� ��������� ������� �����
            static bool IndexFirstPoint(int *first_out, int *skipped_out);

            // ������� � �������� num_words ������ �� ������ address
            static void UtilizeFirstBytes(const uint16 * const address, int num_words);

            // ������������ ������ �����, � ������� ����� ���������� ��������� ������
            static uint8 *CalculateFirstAddressWrite(DataReading &dr, const ::Channel &ch);
        };
    };
};
