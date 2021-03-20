#pragma once
#include "Measures.h"
#include "FPGA/FPGA_Types.h"


class Processing
{
public:

    // ”становить сигнал дл€ обработки.
    static void SetSignal(puchar data0, puchar data1, DataSettings *ds, int firstPoint, int lastPoint);

    // ѕолучить данные ранее установленного сигнала.
    static void GetData(uint8 **data0, uint8 **data1, DataSettings **ds);

    // ѕолучить позицию курсора напр€жени€, соответствующю заданной позиции курсора posCurT.
    static float GetCursU(Channel::E ch, float posCurT);

    // ѕолучить позицию курсора времени, соответствующую заданной позиции курсора напр€жени€ posCurU.
    static float GetCursT(Channel::E ch, float posCurU, int numCur);

    // јппроксимировать единичное измерение режима рандомизатора функцией sinX/X.
    static void InterpolationSinX_X(uint16 data[FPGA_MAX_POINTS], TBase::E tBase);

    // ¬озвращает строку автоматического измерени€.
    static String GetStringMeasure(Measure::E measure, const Channel &ch);

    // –асчитать все измерени€.
    static void CalculateMeasures();

    // ¬озвращает значение горизонтального маркера. ≈сли ERROR_VALUE_INT - маркер рисовать не нужно.
    static int GetMarkerHorizontal(Channel::E ch, int numMarker);

    // ¬озвращает значение вертикального маркера. ≈сли ERROR_VALUE_INT - маркер рисовать не нужно.
    static int GetMarkerVertical(Channel::E ch, int numMarker);

private:

    static float CalculateVoltageMax(Channel::E ch);
    static float CalculateVoltageMin(Channel::E ch);
    static float CalculateVoltagePic(Channel::E ch);
    static float CalculateVoltageMaxSteady(Channel::E ch);
    static float CalculateVoltageMinSteady(Channel::E ch);
    static float CalculateVoltageAmpl(Channel::E ch);
    static float CalculateVoltageAverage(Channel::E ch);
    static float CalculateVoltageRMS(Channel::E ch);
    static float CalculateVoltageVybrosPlus(Channel::E ch);
    static float CalculateVoltageVybrosMinus(Channel::E ch);
    static float CalculatePeriod(Channel::E ch);
    // “очно вычисл€ет период или целое число периодов в точках сигнала.
    static int   CalculatePeriodAccurately(Channel::E ch);
    static float CalculateFreq(Channel::E ch);
    static float CalculateTimeNarastaniya(Channel::E ch);
    static float CalculateTimeSpada(Channel::E ch);
    static float CalculateDurationPlus(Channel::E ch);
    static float CalculateDurationMinus(Channel::E ch);
    static float CalculateSkvaznostPlus(Channel::E ch);
    static float CalculateSkvaznostMinus(Channel::E ch);
    // ¬озвращает минимальное значение относительного сигнала    
    static float CalculateMinRel(Channel::E ch);
    // ¬озвращает минимальное установившеес€ значение относительного сигнала
    static float CalculateMinSteadyRel(Channel::E ch);
    // ¬озвращает максимальное значение относительного сигнала
    static float CalculateMaxRel(Channel::E ch);
    // ¬озвращает максимальное установившеес€ значение относительного сигнала
    static float CalculateMaxSteadyRel(Channel::E ch);
    // ¬озвращает среденее значение относительного сигнала
    static float CalculateAverageRel(Channel::E ch);
    static float CalculatePicRel(Channel::E ch);
    static float CalculateDelayPlus(Channel::E ch);
    static float CalculateDelayMinus(Channel::E ch);
    static float CalculatePhazaPlus(Channel::E ch);
    static float CalculatePhazaMinus(Channel::E ch);
    // Ќайти точку пересечени€ сигнала с горизонтальной линией, проведЄнной на уровне yLine. numItersection - пор€дковый номер пересечени€, 
    // начинаетс€ с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
    static float FindIntersectionWithHorLine(Channel::E ch, int numIntersection, bool downToUp, uint8 yLine);

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
