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

    // ���������� ������ ��� ���������.
    static void SetSignal(Buffer<uint8> &pointsA, Buffer<uint8> &pointsB, DataSettings *ds);

    // ��������� ��� ���������.
    static void CalculateMeasures();

    // �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT.
    static float GetCursU(const Channel &ch, float posCurT);

    // �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU.
    static float GetCursT(const Channel &ch, float posCurU, int numCur);

    // ���������������� ��������� ��������� ������ ������������� �������� sinX/X.
    static void InterpolationSinX_X(Buffer<uint16> &buffer, TBase::E tBase);

    // ���������� ������ ��������������� ���������.
    static Text GetStringMeasure(Measure::E measure, const Channel &ch);

    // ���������� �������� ��������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
    static int GetMarkerHorizontal(Channel::E ch, int numMarker);

    // ���������� �������� ������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
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
    // ����� ��������� ������ ��� ����� ����� �������� � ������ �������.
    static Int   CalculatePeriodAccurately(Channel::E ch);
    static Float CalculateFreq(Channel::E ch);
    static Float CalculateTimeNarastaniya(Channel::E ch);
    static Float CalculateTimeSpada(Channel::E ch);
    static Float CalculateDurationPlus(Channel::E ch);
    static Float CalculateDurationMinus(Channel::E ch);
    static Float CalculateSkvaznostPlus(Channel::E ch);
    static Float CalculateSkvaznostMinus(Channel::E ch);
    // ���������� ����������� �������� �������������� �������    
    static Float CalculateMinRel(Channel::E ch);
    // ���������� ����������� �������������� �������� �������������� �������
    static float CalculateMinSteadyRel(Channel::E ch);
    // ���������� ������������ �������� �������������� �������
    static Float CalculateMaxRel(Channel::E ch);
    // ���������� ������������ �������������� �������� �������������� �������
    static float CalculateMaxSteadyRel(Channel::E ch);
    // ���������� �������� �������� �������������� �������
    static Float CalculateAverageRel(Channel::E ch);
    static Float CalculatePicRel(Channel::E ch);
    static Float CalculateDelayPlus(Channel::E ch);
    static Float CalculateDelayMinus(Channel::E ch);
    static Float CalculatePhazaPlus(Channel::E ch);
    static Float CalculatePhazaMinus(Channel::E ch);
    // ����� ����� ����������� ������� � �������������� ������, ���������� �� ������ yLine. numItersection - ���������� ����� �����������, 
    // ���������� � 1. downToTop - ���� true, ���� ����������� ������� �� ������� ������ ��� ����������� �� "-" � "+".
    static Float FindIntersectionWithHorLine(Channel::E ch, int numIntersection, bool downToUp, uint8 yLine);

    static void CountedToCurrentSettings();

    typedef Float(*pFuncCalculate)(Channel::E);

    struct MeasureCalculate
    {
        char          *name;
        pFuncCalculate FuncCalculate;
        pFuncCFBC      FucnConvertate;
        // ���� true, ����� ���������� ����.
        bool           showSign;
    };

    static const MeasureCalculate measures[Measure::Count];
};
