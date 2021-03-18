// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"


struct ReaderFPGA
{
    static void ClearData();

    static void ReadData();

    static void ReadRandomizeMode();

    static void ReadRealMode();

    static void ReadPoint();

    static DataSettings ds;

    static uint8 data_a[FPGA_MAX_POINTS];       // ����� ������������ ��� ������ ������ ������� ������
    static uint8 data_b[FPGA_MAX_POINTS];       // ����� ������������ ��� ������ ������ ������� ������

    static int addition_shift;                  // �������������� ��������. ����� ��� ���������� ����������� �����
                                                // � ������ �������������

private:

    // ����������� ������.
    static void InverseDataIsNecessary(Channel::E ch, uint8 *data);

    static int CalculateShift();

    static void SaveToStorage();

    static void ReadRealModePeakDetOn();

    static void ReadRealModePeakDetOff();

    static uint16 ReadAddressStop();

    static void ReadChannel(uint8 *data, Channel::E ch, uint16 addr_stop);
};
