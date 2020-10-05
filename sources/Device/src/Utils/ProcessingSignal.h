#pragma once
#include "Measures.h"
#include "defines.h"
#include "FPGA/FPGA_Types.h"


class Processing
{
public:
    // ���������� ������ ��� ���������.
    static void SetSignal(uint8 *data0, uint8 *data1, DataSettings *ds, int firstPoint, int lastPoint);
    // �������� ������ ����� �������������� �������.
    static void GetData(uint8 **data0, uint8 **data1, DataSettings **ds);
    // �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT.
    static float GetCursU(Channel::E chan, float posCurT);
    // �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU.
    static float GetCursT(Channel::E chan, float posCurU, int numCur);
    // ���������������� ��������� ��������� ������ ������������� �������� sinX/X.
    static void InterpolationSinX_X(uint8 data[FPGA_MAX_POINTS], TBase::E tBase);
    // ���������� ������ ��������������� ���������.
    static char* GetStringMeasure(Measure::E measure, Channel::E chan, char buffer[20]);
    // ��������� ��� ���������.
    static void CalculateMeasures();
    // ���������� �������� ��������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
    static int GetMarkerHorizontal(Channel::E chan, int numMarker);
    // ���������� �������� ������������� �������. ���� ERROR_VALUE_INT - ������ �������� �� �����.
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
    // ����� ��������� ������ ��� ����� ����� �������� � ������ �������.
    static int   CalculatePeriodAccurately(Channel::E chan);
    static float CalculateFreq(Channel::E chan);
    static float CalculateTimeNarastaniya(Channel::E chan);
    static float CalculateTimeSpada(Channel::E chan);
    static float CalculateDurationPlus(Channel::E chan);
    static float CalculateDurationMinus(Channel::E chan);
    static float CalculateSkvaznostPlus(Channel::E chan);
    static float CalculateSkvaznostMinus(Channel::E chan);
    // ���������� ����������� �������� �������������� �������    
    static float CalculateMinRel(Channel::E chan);
    // ���������� ����������� �������������� �������� �������������� �������
    static float CalculateMinSteadyRel(Channel::E chan);
    // ���������� ������������ �������� �������������� �������
    static float CalculateMaxRel(Channel::E chan);
    // ���������� ������������ �������������� �������� �������������� �������
    static float CalculateMaxSteadyRel(Channel::E chan);
    // ���������� �������� �������� �������������� �������
    static float CalculateAverageRel(Channel::E chan);
    static float CalculatePicRel(Channel::E chan);
    static float CalculateDelayPlus(Channel::E chan);
    static float CalculateDelayMinus(Channel::E chan);
    static float CalculatePhazaPlus(Channel::E chan);
    static float CalculatePhazaMinus(Channel::E chan);
    // ����� ����� ����������� ������� � �������������� ������, ���������� �� ������ yLine. numItersection - ���������� ����� �����������, 
    // ���������� � 1. downToTop - ���� true, ���� ����������� ������� �� ������� ������ ��� ����������� �� "-" � "+".
    static float FindIntersectionWithHorLine(Channel::E chan, int numIntersection, bool downToUp, uint8 yLine);

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
