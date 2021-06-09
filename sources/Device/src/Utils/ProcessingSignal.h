// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/Buffer_.h"
#include "common/Utils/Containers/Values_.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Types.h"
#include "Utils/Measures.h"


struct DataSettings;


class Processing
{
public:

    // ”становить сигнал дл€ обработки.
    static void SetSignal(Buffer<uint8> &pointsA, Buffer<uint8> &pointsB, DataSettings *ds);

    // –асчитать все измерени€.
    static void CalculateMeasures();

    // ѕолучить позицию курсора напр€жени€, соответствующю заданной позиции курсора posCurT.
    static float GetCursU(const Channel &ch, float posCurT);

    // ѕолучить позицию курсора времени, соответствующую заданной позиции курсора напр€жени€ posCurU.
    static float GetCursT(const Channel &ch, float posCurU, int numCur);

    // јппроксимировать единичное измерение режима рандомизатора функцией sinX/X.
    static void InterpolationSinX_X(Buffer<uint16> &buffer, TBase::E tBase);

    // ¬озвращает строку автоматического измерени€.
    static Text GetStringMeasure(Measure::E measure, const Channel &ch);

    // ¬озвращает значение горизонтального маркера. ≈сли ERROR_VALUE_INT - маркер рисовать не нужно.
    static int GetMarkerHorizontal(Channel::E ch, int numMarker);

    // ¬озвращает значение вертикального маркера. ≈сли ERROR_VALUE_INT - маркер рисовать не нужно.
    static int GetMarkerVertical(Channel::E ch, int numMarker);

private:

    static Float CalculateVoltageMax(Channel::E ch);
    static Float CalculateVoltageMin(Channel::E ch);
    static Float CalculateVoltagePic(Channel::E ch);
    static Float CalculateVoltageMaxSteady(Channel::E ch);
    static Float CalculateVoltageMinSteady(Channel::E ch);
    static Float CalculateVoltageAmpl(Channel::E ch);
    static Float CalculateVoltageAverage(Channel::E ch);
    static Float CalculateVoltageRMS(Channel::E ch);
    static Float CalculateVoltageVybrosPlus(Channel::E ch);
    static Float CalculateVoltageVybrosMinus(Channel::E ch);
    static Float CalculatePeriod(Channel::E ch);
    // “очно вычисл€ет период или целое число периодов в точках сигнала.
    static Int   CalculatePeriodAccurately(Channel::E ch);
    static Float CalculateFreq(Channel::E ch);
    static Float CalculateTimeNarastaniya(Channel::E ch);
    static Float CalculateTimeSpada(Channel::E ch);
    static Float CalculateDurationPlus(Channel::E ch);
    static Float CalculateDurationMinus(Channel::E ch);
    static Float CalculateSkvaznostPlus(Channel::E ch);
    static Float CalculateSkvaznostMinus(Channel::E ch);
    // ¬озвращает минимальное значение относительного сигнала    
    static Float CalculateMinRel(Channel::E ch);
    // ¬озвращает минимальное установившеес€ значение относительного сигнала
    static float CalculateMinSteadyRel(Channel::E ch);
    // ¬озвращает максимальное значение относительного сигнала
    static Float CalculateMaxRel(Channel::E ch);
    // ¬озвращает максимальное установившеес€ значение относительного сигнала
    static float CalculateMaxSteadyRel(Channel::E ch);
    // ¬озвращает среденее значение относительного сигнала
    static Float CalculateAverageRel(Channel::E ch);
    static Float CalculatePicRel(Channel::E ch);
    static Float CalculateDelayPlus(Channel::E ch);
    static Float CalculateDelayMinus(Channel::E ch);
    static Float CalculatePhazaPlus(Channel::E ch);
    static Float CalculatePhazaMinus(Channel::E ch);
    // Ќайти точку пересечени€ сигнала с горизонтальной линией, проведЄнной на уровне yLine. numItersection - пор€дковый номер пересечени€, 
    // начинаетс€ с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
    static Float FindIntersectionWithHorLine(Channel::E ch, int numIntersection, bool downToUp, uint8 yLine);

    static void CountedToCurrentSettings();

    typedef Float(*pFuncCalculate)(Channel::E);

    struct MeasureCalculate
    {
        char          *name;
        pFuncCalculate FuncCalculate;
        pFuncCFBC      FucnConvertate;
        // ≈сли true, нужно показывать знак.
        bool           showSign;
    };

    static const MeasureCalculate measures[Measure::Count];
};
