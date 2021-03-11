 /** @file FPGA_Types.h
    @brief ���� �������� �������, ������ � � ���������, ����������� ��� ������ � ��������
    @verbatim
    ����� ������� ��������� ������� ���������� ��������� ������� 2-15
    ������� WR_START ����������� ������ ��� ��� ������� ����� ��������������
    ������� WR_STOP ����������� ��� ����������� ����� �������������� ����� ����� ������


    ��� ���������� �������� ������� � �������������, ���������� ������� � ������������� ������������ ������ PG2, PG3, PG5, PG7
    PG5 - ����� ������� (���������)
    PG7 - ����� �������� (���������)
    PG2 - �������� ��������
    PG3 - ������

    �������� ������� ������ - ������.
    ����� ������� ���������� ������� ��������� �������� ������ ���� ����� ����.
    ������ ���������� �� ����� ��������� ��������.
    ���������� AD5314.
                            __      __      __      __         __      __      __      __      __
                           /  \    /  \    /  \    /  \       /  \    /  \    /  \    /  \    /  \
    PG2       ____________/    \__/    \__/    \__/    \__/  /    \__/    \__/    \__/    \__/    \_____________
              _________                                                                                _________
    PG5 (PG7)          \___________________________________   ________________________________________/
                             ____    ____    ____    ____       ____    ____    ____    ____    ____                                                              
    PG3       ______________/    \__/    \__/    \__/    \_   _/    \__/    \__/    \__/    \__/    \___________
                            \____/  \____/  \____/  \____/     \____/  \____/  \____/  \____/  \____/                                                    
                             DB15    DB14    DB13    DB12       DB4     DB3     DB2     DB1     DB0

    @endverbatim 
*/

#pragma once
#include "Display/Grid.h"


#define RShiftMin   20          //    ����������� �������� �������� ������ �� ����������, ���������� � ���������� �����. ������������� �������� 10 ������ ���� �� ����������� �����.
#define RShiftZero  500         //    ������� �������� �������� ������ �� ����������, ���������� � ���������� �����. ������������� ������������ ������ �� ������ ������.
#define RShiftMax   980         //    ������������ �������� �������� ������ �� ����������, ���������� � ���������� �����. ������������� �������� 10 ������ ����� �� ����������� �����.
#define STEP_RSHIFT static_cast<int16>(((RShiftMax - RShiftMin) / 24) / Grid::Delta())   // �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� �� ���������� ������������ �� ���� �����.

#define TrigLevMin  RShiftMin   //    ����������� �������� ������ �������������, ���������� � ������.
#define TrigLevMax  RShiftMax   //    ������������ ���������� ������ �������������, ���������� � ���������� �����.
#define TrigLevZero RShiftZero  //    ������� �������� ������ �������������, ���������� � ������. ������ ������������� ��� ���� ���������� �� ����� ������ � �������� �������� �� ����������.

#define TShiftMax   16383       //    ����������� �������� �������� �� ������� ������������ ����� �������������, ���������� � �������.

//    ��� �������� �������� �������, ��������� � ���, ������������� ������ ������� ����� (-5 ������ �� ������). ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MIN_VALUE   27
#define AVE_VALUE   127         //    ��� �������� �������� �������, ��������� � ���, ������������� ������ �����. ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MAX_VALUE   227         //    ��� �������� �������� �������, ��������� � ���, ������������� ������� ������� ����� (+5 ������ �� ������). ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������.
#define MIN_VALUE_SCREEN 0
#define MAX_VALUE_SCREEN 200

#define MIN_TBASE_PEC_DEAT  TBase::_500ns     // ����������� ������� �� �������, ��� ������� �������� ��������� ������ �������� ���������.
#define MIN_TBASE_P2P       TBase::_20ms      // � ����� �������� tBase ������ ���������� ����� ����������� ������.

#define FPGA_MAX_POINTS     1024            
#define FPGA_MAX_POINTS_FOR_CALCULATE 900


#define SHIFT(x) (0x02 * (x))


#define WR_START        (HAL_FMC::ADDR_FPGA + SHIFT(0))
#define WR_RAZV         (HAL_FMC::ADDR_FPGA + SHIFT(1))
#define WR_PRED         (HAL_FMC::ADDR_FPGA + SHIFT(2))
#define WR_POST         (HAL_FMC::ADDR_FPGA + SHIFT(3))
#define WR_TRIG         (HAL_FMC::ADDR_FPGA + SHIFT(4))
#define WR_CAL_A        (HAL_FMC::ADDR_FPGA + 12)            //    ������������� ����������� ������ 1.
#define WR_CAL_B        (HAL_FMC::ADDR_FPGA + 13)            //    ������������� ����������� ������ 2.
                                                    /**     @brief ���������� ������������, ������� ���������� � ��������������.
                                                            @verbatim
                                                            D0 - ����� ������������� � 1 ��� ��������������� ��������� ��� ������������ �������,\n
                                                            D1 - ������� �������� ���./����. ����� �������� �
                                                            D2, D3 - ���������� : D2 == 1 - 1���, D3 == 1 - ����, D2 == 0, D3 == 0 - ��
                                                            @endverbatim */
#define WR_UPR          (HAL_FMC::ADDR_FPGA + 14)
#define UPR_BIT_PEAKDET             1               // ������� �������� - 0/1 ����/���
#define UPR_BIT_CALIBRATOR_AC_DC    2               // ����������/����������
#define UPR_BIT_CALIBRATOR_VOLTAGE  3               // 0/4�
                                                    /** @brief �������������� ���������� ����� ���������. 
                                                        @verbatim
                                                        ��� ����, ����� �������� ����� ������ ����, ����� ��� ���� ���������, ������� ����� ������� �� ������ WR_STOP 1, ����� ����� ���������, � ����� ����� ���� ������� WR_START.
                                                        @endverbatim */
#define WR_STOP          (HAL_FMC::ADDR_FPGA + 15)            


#define RD_ADC_A1       (HAL_FMC::ADDR_FPGA + 0x10)          //    ������ �� ������ ��� ������ 1 1-�� �����.
#define RD_ADC_A2       (HAL_FMC::ADDR_FPGA + 0x11)          //    ������ �� ������ ��� ������ 1 2-�� �����.
#define RD_ADC_B1       (HAL_FMC::ADDR_FPGA + 0x12)          //    ������ �� ������ ��� ������ 2 1-�� �����.
#define RD_ADC_B2       (HAL_FMC::ADDR_FPGA + 0x13)          //    ������ �� ������ ��� ������ 2 2-�� �����.
                                                    /**     @brief ������ ������������� ������.
                                                            @verbatim
                                                            D0 - ���� ���������� ������,\n
                                                            D1 - ���� �������������,\n
                                                            D2 -\n
                                                            D3 - ���� ���������� ��������� ����� ��� ����������� ������,\n
                                                            D4 - ���������� ����� �������,\n
                                                            D5 - ���������� ��������� �������,\n
                                                            D6 - ������� ����, ��� ����� ������� ������� �� 1 ����� � �������. 
                                                            @endverbatim */
#define RD_FL           (HAL_FMC::ADDR_FPGA + 0x16)
#define BIT_DATA_READY          0
#define BIT_TRIG                1
#define BIT_POINT_READY         3
#define BIT_FREQ_READY          4
#define BIT_PERIOD_READY        5
#define BIT_SIGN_SHIFT_POINT    6


#define RD_LAST_RECORD_LOW  (HAL_FMC::ADDR_FPGA + 0x17)      //    ������� ���� ������ ��������� ������. ������� ��������� � RD_LAST_RECORD_HI. ������������ � ������ ����������� ������, ����� �� ��������� ��� �����, ����� ������������ �� ����� 1024. ��� �������� ������.
#define RD_LAST_RECORD_HI   (HAL_FMC::ADDR_FPGA + 0x18)      //    ������� ���� ������ ��������� ������. ������� ��������� � RD_LAST_RECORD_LOW. ������������ � ������ ����������� ������, ����� �� ��������� ��� �����, ����� ������������ �� ����� 1024. ��� �������� ������.
#define WR_ADD_RSHIFT_DAC2  (HAL_FMC::ADDR_FPGA + 0x1b)      //    �������������� �������� ��� 2-�� �����. �.�. ���� ����� ��������� ��� ��� ��������� � ��� ����������������, ���������� ���� ���������� �������� ��� ����������� ���������������.
#define WR_ADD_RSHIFT_DAC1  (HAL_FMC::ADDR_FPGA + 0x1c)      //    �������������� �������� ��� 1-�� �����. �.�. ���� ����� ��������� ��� ��� ��������� � ��� ����������������, ���������� ���� ���������� �������� ��� ����������� ���������������.

#define RD_ADDR_PERIOD_LOW_LOW  (HAL_FMC::ADDR_FPGA + 0x19)  //    0-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D4 RD_FL.
#define RD_ADDR_PERIOD_LOW      (HAL_FMC::ADDR_FPGA + 0x1a)  //    1-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D4 RD_FL.
#define RD_ADDR_PERIOD_MID      (HAL_FMC::ADDR_FPGA + 0x1b)  //    2-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D4 RD_FL.
#define RD_ADDR_PERIOD_HI       (HAL_FMC::ADDR_FPGA + 0x1c)  //    3-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D4 RD_FL.


#define RD_ADDR_FREQ_LOW    (HAL_FMC::ADDR_FPGA + 0x1d)      //    0-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D5 RD_FL.
#define RD_ADDR_FREQ_MID    (HAL_FMC::ADDR_FPGA + 0x1e)      //    1-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D5 RD_FL.
#define RD_ADDR_FREQ_HI     (HAL_FMC::ADDR_FPGA + 0x1f)      //    2-� ���� ����������� �������� �������. ������� ����, ��� ���� ������� ��������, ��������� � D5 RD_FL.

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
