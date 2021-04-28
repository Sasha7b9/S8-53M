// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Containers/Values_.h"


#define FPGA_MAX_POINTS_FOR_CALCULATE   900


#define WR_START                (HAL_FMC::ADDR_FPGA + 0x00)
#define WR_RAZV                 (HAL_FMC::ADDR_FPGA + 0x01)
#define WR_PRED                 (HAL_FMC::ADDR_FPGA + 0x02)
#define WR_POST                 (HAL_FMC::ADDR_FPGA + 0x03)
#define WR_TRIG                 (HAL_FMC::ADDR_FPGA + 0x04)
#define WR_UPR                  (HAL_FMC::ADDR_FPGA + 0x05)
#define UPR_BIT_PEAKDET                 1   // пиковый детектор - 0/1 выкл/вкл
#define UPR_BIT_CALIBRATOR_AC_DC        2   // посто€нное/переменное
#define UPR_BIT_CALIBRATOR_VOLTAGE      3   // 0/4¬
#define UPR_BIT_RECORDER                4   // 0 - обычный режим, 1 - регистратор
#define WR_ADDR_READ            (HAL_FMC::ADDR_FPGA + 0x06)     // Ёта команда записывает адрес чтени€ в регистр предзапуска
#define WR_STOP                 (HAL_FMC::ADDR_FPGA + 0x1f)


#define RD_ADC_A                (HAL_FMC::ADDR_FPGA + 0x00)
#define RD_ADC_B                (HAL_FMC::ADDR_FPGA + 0x02)
#define RD_ADDR_LAST_RECORD     (HAL_FMC::ADDR_FPGA + 0x08)     // „тение адреса последней записи
#define RD_FREQ_LOW             (HAL_FMC::ADDR_FPGA + 0x10)
#define RD_FREQ_HI              (HAL_FMC::ADDR_FPGA + 0x18)
#define RD_PERIOD_LOW           (HAL_FMC::ADDR_FPGA + 0x20)
#define RD_PERIOD_HI            (HAL_FMC::ADDR_FPGA + 0x28)
#define RD_FL                   (HAL_FMC::ADDR_FPGA + 0x30)
#define FL_DATA_READY                   0   // 0 - данные готовы
#define FL_TRIG_READY                   1   // 1 - наличие синхроимпульса
#define FL_PRED_READY                   2   // 2 - окончание счета предзапуска
#define FL_POINT_READY                  3   // 3 - признак того, что точка готова (в поточечном выводе)
#define FL_FREQ_READY                   4   // 4 - можно считывать частоту
#define FL_PERIOD_READY                 5   // 5 - можно считывать период
#define FL_LAST_RECOR                   6   // 6 - признак последней записи - определ€ет, какой бит ставить первым
#define FL_OVERFLOW_FREQ                8   // 8 - признак переполнени€ счЄтчика частоты
#define FL_OVERFLOW_PERIOD              9   // 9 - признак переполнени€ счЄтчика периода


struct TypeWriteAnalog { enum E
{
    All,
    RangeA,
    RangeB,
    TrigParam,
    ChanParamA,
    ChanParamB
};};

struct TypeWriteDAC { enum E
{
    RShiftA,
    RShiftB,
    TrigLev
};};


struct Value
{
    static const uint8 MIN = 2;
    static const uint8 AVE = 127;
    static const uint8 MAX = 252;
    static const uint8 NONE = 0;

    static void ToVoltage(puchar points, int numPoints, Range::E range, int16 rShift, float *voltage);
    static Voltage ToVoltage(uint8 value, Range::E range, int16 rshift);
    static void FromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);
    static uint8 FromVoltage(const Voltage &voltage, Range::E range, int16 rshift);
};


extern const uint16 * const addresses_ADC[2];
#define ADDRESS_READ(ch) addresses_ADC[ch]
