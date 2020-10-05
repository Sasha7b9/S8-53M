#pragma once
#include "Measures.h"
#include "defines.h"
#include "FPGA/FPGA_Types.h"


class Processing
{
public:
    // ”становить сигнал дл€ обработки.
    static void SetSignal(uint8 *data0, uint8 *data1, DataSettings *ds, int firstPoint, int lastPoint);
    // ѕолучить данные ранее установленного сигнала.
    static void GetData(uint8 **data0, uint8 **data1, DataSettings **ds);
    // ѕолучить позицию курсора напр€жени€, соответствующю заданной позиции курсора posCurT.
    static float GetCursU(Channel::E chan, float posCurT);
    // ѕолучить позицию курсора времени, соответствующую заданной позиции курсора напр€жени€ posCurU.
    static float GetCursT(Channel::E chan, float posCurU, int numCur);
    // јппроксимировать единичное измерение режима рандомизатора функцией sinX/X.
    static void InterpolationSinX_X(uint8 data[FPGA_MAX_POINTS], TBase::E tBase);
    // ¬озвращает строку автоматического измерени€.
    static char* GetStringMeasure(Measure::E measure, Channel::E chan, char buffer[20]);
    // –асчитать все измерени€.
    static void CalculateMeasures();
    // ¬озвращает значение горизонтального маркера. ≈сли ERROR_VALUE_INT - маркер рисовать не нужно.
    static int GetMarkerHorizontal(Channel::E chan, int numMarker);
    // ¬озвращает значение вертикального маркера. ≈сли ERROR_VALUE_INT - маркер рисовать не нужно.
    static int GetMarkerVertical(Channel::E chan, int numMarker);
private:
    static float CalculateVoltageMax(Channel::E chan);
    static float CalculateVoltageMin(Channel::E chan);
    static float CalculateVoltagePic(Channel::E chan);
    static float CalculateVoltageMaxSteady(Channel::E chan);
    static float CalculateVoltageMinSteady(Channel::E chan);
    static float CalculateVoltageAmpl(Channel::E chan);
    static float CalculateVoltageAverage(Channel::E chan);
    static float CalculateVoltageRMS(Channel::E chan);
    static float CalculateVoltageVybrosPlus(Channel::E chan);
    static float CalculateVoltageVybrosMinus(Channel::E chan);
    static float CalculatePeriod(Channel::E chan);
    // “очно вычисл€ет период или целое число периодов в точках сигнала.
    static int   CalculatePeriodAccurately(Channel::E chan);
    static float CalculateFreq(Channel::E chan);
    static float CalculateTimeNarastaniya(Channel::E chan);
    static float CalculateTimeSpada(Channel::E chan);
    static float CalculateDurationPlus(Channel::E chan);
    static float CalculateDurationMinus(Channel::E chan);
    static float CalculateSkvaznostPlus(Channel::E chan);
    static float CalculateSkvaznostMinus(Channel::E chan);
    // ¬озвращает минимальное значение относительного сигнала    
    static float CalculateMinRel(Channel::E chan);
    // ¬озвращает минимальное установившеес€ значение относительного сигнала
    static float CalculateMinSteadyRel(Channel::E chan);
    // ¬озвращает максимальное значение относительного сигнала
    static float CalculateMaxRel(Channel::E chan);
    // ¬озвращает максимальное установившеес€ значение относительного сигнала
    static float CalculateMaxSteadyRel(Channel::E chan);
    // ¬озвращает среденее значение относительного сигнала
    static float CalculateAverageRel(Channel::E chan);
    static float CalculatePicRel(Channel::E chan);
    static float CalculateDelayPlus(Channel::E chan);
    static float CalculateDelayMinus(Channel::E chan);
    static float CalculatePhazaPlus(Channel::E chan);
    static float CalculatePhazaMinus(Channel::E chan);
    // Ќайти точку пересечени€ сигнала с горизонтальной линией, проведЄнной на уровне yLine. numItersection - пор€дковый номер пересечени€, 
    // начинаетс€ с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
    static float FindIntersectionWithHorLine(Channel::E chan, int numIntersection, bool downToUp, uint8 yLine);

    static void CountedToCurrentSettings();

    typedef float (*pFuncFCh)(Channel::E);

    struct MeasureCalculate
    {
        char        *name;
        pFuncFCh    FuncCalculate;
        pFuncCFBC   FucnConvertate;
        // ≈сли true, нужно показывать знак.
        bool        showSign;
    };

    static const MeasureCalculate measures[Measure::Count];
};
