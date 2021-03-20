#pragma once
#include "Measures.h"
#include "FPGA/FPGA_Types.h"


class Processing
{
public:

    // ���������� ������ ��� ���������.
    static void SetSignal(puchar data0, puchar data1, DataSettings *ds, int firstPoint, int lastPoint);

    // �������� ������ ����� �������������� �������.
    static void GetData(uint8 **data0, uint8 **data1, DataSettings **ds);

    // �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT.
    static float GetCursU(Channel::E ch, float posCurT);

    // �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU.
    static float GetCursT(Channel::E ch, float posCurU, int numCur);

    // ���������������� ��������� ��������� ������ ������������� �������� sinX/X.
    static void InterpolationSinX_X(uint16 data[FPGA_MAX_POINTS], TBase::E tBase);

    // ���������� ������ ��������������� ���������.
    static String GetStringMeasure(Measure::E measure, const Channel &ch);

    // ��������� ��� ���������.
    static void CalculateMeasures();

    // ���������� �������� ��������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
    static int GetMarkerHorizontal(Channel::E ch, int numMarker);

    // ���������� �������� ������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
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
    // ����� ��������� ������ ��� ����� ����� �������� � ������ �������.
    static int   CalculatePeriodAccurately(Channel::E ch);
    static float CalculateFreq(Channel::E ch);
    static float CalculateTimeNarastaniya(Channel::E ch);
    static float CalculateTimeSpada(Channel::E ch);
    static float CalculateDurationPlus(Channel::E ch);
    static float CalculateDurationMinus(Channel::E ch);
    static float CalculateSkvaznostPlus(Channel::E ch);
    static float CalculateSkvaznostMinus(Channel::E ch);
    // ���������� ����������� �������� �������������� �������    
    static float CalculateMinRel(Channel::E ch);
    // ���������� ����������� �������������� �������� �������������� �������
    static float CalculateMinSteadyRel(Channel::E ch);
    // ���������� ������������ �������� �������������� �������
    static float CalculateMaxRel(Channel::E ch);
    // ���������� ������������ �������������� �������� �������������� �������
    static float CalculateMaxSteadyRel(Channel::E ch);
    // ���������� �������� �������� �������������� �������
    static float CalculateAverageRel(Channel::E ch);
    static float CalculatePicRel(Channel::E ch);
    static float CalculateDelayPlus(Channel::E ch);
    static float CalculateDelayMinus(Channel::E ch);
    static float CalculatePhazaPlus(Channel::E ch);
    static float CalculatePhazaMinus(Channel::E ch);
    // ����� ����� ����������� ������� � �������������� ������, ���������� �� ������ yLine. numItersection - ���������� ����� �����������, 
    // ���������� � 1. downToTop - ���� true, ���� ����������� ������� �� ������� ������ ��� ����������� �� "-" � "+".
    static float FindIntersectionWithHorLine(Channel::E ch, int numIntersection, bool downToUp, uint8 yLine);

    static void CountedToCurrentSettings();

    typedef float (*pFuncFCh)(Channel::E);

    struct MeasureCalculate
    {
        char        *name;
        pFuncFCh    FuncCalculate;
        pFuncCFBC   FucnConvertate;
        // ���� true, ����� ���������� ����.
        bool        showSign;
    };

    static const MeasureCalculate measures[Measure::Count];
};
