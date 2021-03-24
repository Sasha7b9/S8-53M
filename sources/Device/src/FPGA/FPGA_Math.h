#pragma once
#include "common/Settings/SettingsTypes_.h"


#define MAX_VOLTAGE_ON_SCREEN(range) (MathFPGA::tableScalesRange[(range)] * 5.0F)

struct MathFPGA
{
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
