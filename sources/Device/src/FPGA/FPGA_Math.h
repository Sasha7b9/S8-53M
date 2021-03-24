#pragma once
#include "common/Settings/SettingsTypes_.h"


#define MAX_VOLTAGE_ON_SCREEN(range) (MathFPGA::tableScalesRange[(range)] * 5.0F)

struct MathFPGA
{
    static float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);
    static void  PointsRelToVoltage(puchar points, int numPoints, Range::E range, int16 rShift, float *voltage);
    static void	 PointsVoltageToRel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);
    static uint8 VoltageToPoint(float voltage, Range::E range, int16 rShift);
    static float TimeCursor(float shiftCurT, TBase::E tBase);
    static void  CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1,
        float *density1, int *y0, int *y1);
    static Float GetMaxFromArrayWithErrorCode(puchar data, uint firstPoint, uint lastPoint);
    static Float GetMinFromArrayWithErrorCode(puchar data, uint firstPoint, uint lastPoint);
    static void  CalculateMathFunction(float *data0andResult, const float *data1, int numPoints);

    static const float tableScalesRange[Range::Count];
    static const float absStepRShift[]; //-V2504
    static const float voltsInPixel[]; //-V2504
    static const float absStepTShift[]; //-V2504
};
