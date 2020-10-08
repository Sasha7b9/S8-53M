#pragma once
#include "common/Settings/cSettingsTypes.h"


const char *RangeName(Range::E range);

const char *TBaseName(TBase::E tBase);


// Сжимать ли сигналы при выводе измерений.
struct ModeViewSignals { enum E
{
    AsIs,       // Показывать сигналы как есть
    Compress    // Сжимать сетку с сигналами
};};


// Math


struct ScaleMath { enum E
{
    Channel0,     // Масштаб берётся из канала 1
    Channel1,     // Масштаб берётся из канала 2
    Hand          // Масштаб задаётся вручную.
};};


// Тип балансировки АЦП каналов.
// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADCtype { enum E
{
    Disable,     // Балансировка выключена.
    Settings,    // Используются значения балансировки, которые получены автоматически.
    Hand         // Используются значения балансировки, заданные вручную.
};};

// Тип дополнительного смещения
struct RShiftADCtype { enum E
{
    Disable,
    Settings,
    Hand
};};
