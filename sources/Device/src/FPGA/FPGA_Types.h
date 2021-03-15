#pragma once
#include "Display/Grid.h"


#define RShiftMin   20          // ����������� �������� �������� ������ �� ����������, ���������� � ���������� �����.
                                // ������������� �������� 10 ������ ���� �� ����������� �����.
#define RShiftZero  500         // ������� �������� �������� ������ �� ����������, ���������� � ���������� �����.
                                // ������������� ������������ ������ �� ������ ������.
#define RShiftMax   980         // ������������ �������� �������� ������ �� ����������, ���������� � ���������� �����.
                                // ������������� �������� 10 ������ ����� �� ����������� �����.
#define STEP_RSHIFT static_cast<int16>(((RShiftMax - RShiftMin) / 24) / Grid::Delta())   // �� ������� ������ �����
                                // �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ����
                                //�����.
#define TrigLevMin  RShiftMin   // ����������� �������� ������ �������������, ���������� � ������.
#define TrigLevMax  RShiftMax   // ������������ ���������� ������ �������������, ���������� � ���������� �����.
#define TrigLevZero RShiftZero  // ������� �������� ������ �������������, ���������� � ������. ������ ������������� ���
                                // ���� ���������� �� ����� ������ � �������� �������� �� ����������.
#define TShiftMax   16000       // ����������� �������� �������� �� ������� ������������ ����� �������������, ����������
                                //� �������

// ��� �������� �������� �������, ��������� � ���, ������������� ������ ������� ����� (-5 ������ �� ������). ����
// �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift
// ����� ���������� �������.
#define MIN_VALUE   27
#define AVE_VALUE   127         // ��� �������� �������� �������, ��������� � ���, ������������� ������ �����. ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MAX_VALUE   227         // ��� �������� �������� �������, ��������� � ���, ������������� ������� ������� ����� (+5 ������ �� ������). ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MIN_VALUE_SCREEN 0
#define MAX_VALUE_SCREEN 200

#define MIN_TBASE_PEC_DEAT  TBase::_500ns     // ����������� ������� �� �������, ��� ������� �������� ��������� ������ �������� ���������.
#define MIN_TBASE_P2P       TBase::_20ms      // � ����� �������� tBase ������ ���������� ����� ����������� ������.

#define FPGA_MAX_POINTS                 16384
#define FPGA_MAX_POINTS_FOR_CALCULATE   900


#define SHIFT(x) (0x02 * (x))


#define WR_START                (HAL_FMC::ADDR_FPGA + SHIFT(0x00))
#define WR_RAZV                 (HAL_FMC::ADDR_FPGA + SHIFT(0x01))
#define WR_PRED                 (HAL_FMC::ADDR_FPGA + SHIFT(0x02))
#define WR_POST                 (HAL_FMC::ADDR_FPGA + SHIFT(0x03))
#define WR_TRIG                 (HAL_FMC::ADDR_FPGA + SHIFT(0x04))
#define WR_UPR                  (HAL_FMC::ADDR_FPGA + SHIFT(0x05))
#define UPR_BIT_PEAKDET                 1   // ������� �������� - 0/1 ����/���
#define UPR_BIT_CALIBRATOR_AC_DC        2   // ����������/����������
#define UPR_BIT_CALIBRATOR_VOLTAGE      3   // 0/4�
#define UPR_BIT_RECORDER                4   // 0 - ������� �����, 1 - �����������
#define WR_STOP                 (HAL_FMC::ADDR_FPGA + SHIFT(0x1f))


#define RD_ADC_A                (HAL_FMC::ADDR_FPGA + SHIFT(0x00))
#define RD_ADC_B                (HAL_FMC::ADDR_FPGA + SHIFT(0x02))
#define RD_FREQ_LOW             (HAL_FMC::ADDR_FPGA + SHIFT(0x10))
#define RD_FREQ_HI              (HAL_FMC::ADDR_FPGA + SHIFT(0x18))
#define RD_PERIOD_LOW           (HAL_FMC::ADDR_FPGA + SHIFT(0x20))
#define RD_PERIOD_HI            (HAL_FMC::ADDR_FPGA + SHIFT(0x28))
#define RD_FL                   (HAL_FMC::ADDR_FPGA + SHIFT(0x30))
#define FL_DATA_READY           0   // 0 - ������ ������
#define FL_TRIG_READY           1   // 1 - ������� ��������������
#define FL_PRED_READY           2   // 2 - ��������� ����� �����������
#define FL_POINT_READY          3   // 3 - ������� ����, ��� ����� ������ (� ���������� ������)
#define FL_FREQ_READY           4   // 4 - ����� ��������� �������
#define FL_PERIOD_READY         5   // 5 - ����� ��������� ������
#define FL_LAST_RECOR           6   // 6 - ������� ��������� ������ - ����������, ����� ��� ������� ������
#define FL_OVERFLOW_FREQ        8   // 8 - ������� ������������ �������� �������
#define FL_OVERFLOW_PERIOD      9   // 9 - ������� ������������ �������� �������


struct TypeWriteAnalog { enum E
{
    All,
    Range0,
    Range1,
    TrigParam,
    ChanParam0,
    ChanParam1
};};

struct TypeWriteDAC { enum E
{
    RShiftA,
    RShiftB,
    TrigLev
};};
