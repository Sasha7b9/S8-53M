#pragma once
#include "Display/Grid.h"


#define TShiftMax   16000       // ����������� �������� �������� �� ������� ������������ ����� �������������, ����������
                                //� �������

// ��� �������� �������� �������, ��������� � ���, ������������� ������ ������� ����� (-5 ������ �� ������). ����
// �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift
// ����� ���������� �������.
#define MIN_VALUE   27
#define AVE_VALUE   127         // ��� �������� �������� �������, ��������� � ���, ������������� ������ �����. ����
                                // �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � �����������
                                // ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MAX_VALUE   227         // ��� �������� �������� �������, ��������� � ���, ������������� ������� ������� �����
                                // (+5 ������ �� ������). ���� �������� == 0, ������, ��� ���. ��� ����� ��� �������
                                // ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MIN_VALUE_SCREEN 0
#define MAX_VALUE_SCREEN 200

#define MIN_TBASE_PEC_DEAT  TBase::_500ns     // ����������� ������� �� �������, ��� ������� �������� ��������� ������
                                              // �������� ���������.

#define FPGA_MAX_POINTS                 1024
#define FPGA_MAX_POINTS_FOR_CALCULATE   900


#define WR_START                (HAL_FMC::ADDR_FPGA + 0x00)
#define WR_RAZV                 (HAL_FMC::ADDR_FPGA + 0x01)
#define WR_PRED                 (HAL_FMC::ADDR_FPGA + 0x02)
#define WR_POST                 (HAL_FMC::ADDR_FPGA + 0x03)
#define WR_TRIG                 (HAL_FMC::ADDR_FPGA + 0x04)
#define WR_UPR                  (HAL_FMC::ADDR_FPGA + 0x05)
#define UPR_BIT_PEAKDET                 1   // ������� �������� - 0/1 ����/���
#define UPR_BIT_CALIBRATOR_AC_DC        2   // ����������/����������
#define UPR_BIT_CALIBRATOR_VOLTAGE      3   // 0/4�
#define UPR_BIT_RECORDER                4   // 0 - ������� �����, 1 - �����������
#define WR_ADDR_STOP            (HAL_FMC::ADDR_FPGA + 0x06)
#define WR_STOP                 (HAL_FMC::ADDR_FPGA + 0x1f)


#define RD_ADC_A                (HAL_FMC::ADDR_FPGA + 0x00)
#define RD_ADC_B                (HAL_FMC::ADDR_FPGA + 0x02)
#define RD_ADDR_NSTOP           (HAL_FMC::ADDR_FPGA + 0x08)
#define RD_FREQ_LOW             (HAL_FMC::ADDR_FPGA + 0x10)
#define RD_FREQ_HI              (HAL_FMC::ADDR_FPGA + 0x18)
#define RD_PERIOD_LOW           (HAL_FMC::ADDR_FPGA + 0x20)
#define RD_PERIOD_HI            (HAL_FMC::ADDR_FPGA + 0x28)
#define RD_FL                   (HAL_FMC::ADDR_FPGA + 0x30)
#define FL_DATA_READY                   0   // 0 - ������ ������
#define FL_TRIG_READY                   1   // 1 - ������� ��������������
#define FL_PRED_READY                   2   // 2 - ��������� ����� �����������
#define FL_POINT_READY                  3   // 3 - ������� ����, ��� ����� ������ (� ���������� ������)
#define FL_FREQ_READY                   4   // 4 - ����� ��������� �������
#define FL_PERIOD_READY                 5   // 5 - ����� ��������� ������
#define FL_LAST_RECOR                   6   // 6 - ������� ��������� ������ - ����������, ����� ��� ������� ������
#define FL_OVERFLOW_FREQ                8   // 8 - ������� ������������ �������� �������
#define FL_OVERFLOW_PERIOD              9   // 9 - ������� ������������ �������� �������


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


extern uint16 *addresses_ADC[2];
#define ADDRESS_READ(ch) addresses_ADC[ch]
