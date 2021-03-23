#pragma once
#include "Display/Grid.h"


#define TShiftMax   16000       // Максимально значение задержки по времени относительно точки синхронизации, засылаемое
                                //в альтеру

// Это значение входного сигнала, считанное с АЦП, соответствует нижней границе сетки (-5 клеток от центра). Если
// значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift
// ранее считанного сигнала.
#define MIN_VALUE   27
#define AVE_VALUE   127         // Это значение входного сигнала, считанное с АЦП, соответствует центру сетки. Если
                                // значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного
                                // вывода p2p, а также для tShift ранее считанного сигнала.
#define MAX_VALUE   227         // Это значение входного сигнала, считанное с АЦП, соответствует верхней границе сетки
                                // (+5 клеток от центра). Если значение == 0, значит, его нет. Это нужно для режимов
                                // рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала.
#define MIN_VALUE_SCREEN 0
#define MAX_VALUE_SCREEN 200

#define MIN_TBASE_PEC_DEAT  TBase::_500ns     // Минимальный масштаб по времени, при котором возможно включение режима
                                              // пикового детектора.

#define FPGA_MAX_POINTS                 1024
#define FPGA_MAX_POINTS_FOR_CALCULATE   900


#define WR_START                (HAL_FMC::ADDR_FPGA + 0x00)
#define WR_RAZV                 (HAL_FMC::ADDR_FPGA + 0x01)
#define WR_PRED                 (HAL_FMC::ADDR_FPGA + 0x02)
#define WR_POST                 (HAL_FMC::ADDR_FPGA + 0x03)
#define WR_TRIG                 (HAL_FMC::ADDR_FPGA + 0x04)
#define WR_UPR                  (HAL_FMC::ADDR_FPGA + 0x05)
#define UPR_BIT_PEAKDET                 1   // пиковый детектор - 0/1 выкл/вкл
#define UPR_BIT_CALIBRATOR_AC_DC        2   // постоянное/переменное
#define UPR_BIT_CALIBRATOR_VOLTAGE      3   // 0/4В
#define UPR_BIT_RECORDER                4   // 0 - обычный режим, 1 - регистратор
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
#define FL_DATA_READY                   0   // 0 - данные готовы
#define FL_TRIG_READY                   1   // 1 - наличие синхроимпульса
#define FL_PRED_READY                   2   // 2 - окончание счета предзапуска
#define FL_POINT_READY                  3   // 3 - признак того, что точка готова (в поточечном выводе)
#define FL_FREQ_READY                   4   // 4 - можно считывать частоту
#define FL_PERIOD_READY                 5   // 5 - можно считывать период
#define FL_LAST_RECOR                   6   // 6 - признак последней записи - определяет, какой бит ставить первым
#define FL_OVERFLOW_FREQ                8   // 8 - признак переполнения счётчика частоты
#define FL_OVERFLOW_PERIOD              9   // 9 - признак переполнения счётчика периода


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
