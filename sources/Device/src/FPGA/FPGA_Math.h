// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Containers/Values_.h"


#define MAX_VOLTAGE_ON_SCREEN(range) (MathFPGA::tableScalesRange[(range)] * 5.0F)

struct MathFPGA
{
    static void  CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1,
        float *density1, int *y0, int *y1);

    static Float GetMaxFromArrayWithErrorCode(puchar data, uint firstPoint, uint lastPoint);

    static Float GetMinFromArrayWithErrorCode(puchar data, uint firstPoint, uint lastPoint);

    static void  CalculateMathFunction(float *data0andResult, const float *data1, int numPoints);

    // Сдвинуть буфер на shift элементов вправо или влево. При этом потерянные элементы заменяются ближайшими к ним
    template<class T>
    static void ShiftBuffer(T *const first, T *const last, int shift);

    static const float tableScalesRange[Range::Count];
    static const float voltsInPixel[];
};
