#pragma once
#include "Settings/SettingsTypes.h"


#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5.0F)

#define RSHIFT_2_ABS(rShift, range) (-((float)RShiftZero - (float)(rShift)) * absStepRShift[(uint)(range)])


extern const float tableScalesRange[Range::Count];
extern const float absStepRShift[]; //-V2504
extern const float voltsInPixel[]; //-V2504
extern const float absStepTShift[]; //-V2504


namespace Math
{
    int   RShift2Rel(float rShiftAbs, Range::E range);
    float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);
    void    PointsRelToVoltage(pUCHAR points, int numPoints, Range::E range, int16 rShift, float* voltage);
    void	PointsVoltageToRel(const float* voltage, int numPoints, Range::E range, int16 rShift, uint8* points);
    uint8   VoltageToPoint(float voltage, Range::E range, int16 rShift);
    float   TimeCursor(float shiftCurT, TBase::E tBase);
    void    CalculateFFT(float* data, int numPoints, float* result, float* freq0, float* density0, float* freq1, float* density1, int* y0, int* y1);
    uint8   GetMaxFromArrayWithErrorCode(pUCHAR data, int firstPoint, int lastPoint);
    uint8   GetMinFromArrayWithErrorCode(pUCHAR data, int firstPoint, int lastPoint);
    void    CalculateMathFunction(float* data0andResult, const float* data1, int numPoints);
}
