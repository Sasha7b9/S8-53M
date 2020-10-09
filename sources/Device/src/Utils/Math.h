#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


#define LIMIT_BELOW(x, min)         if(x < min) { x = min; }

#define LIMIT_ABOVE(x, max)         if(x > max) { x = max; }

#define SET_MIN_IF_LESS(x, min)     if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)   if((x) > (max)) { (max) = (x); }

#define LIMITATION(var, value, min, max)    var = (value); if(var < (min)) { var = (min); } else if(var > (max)) { var = (max); };

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };

#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5.0F)

#define RSHIFT_2_ABS(rShift, range) (-((float)RShiftZero - (float)(rShift)) * absStepRShift[(uint)(range)])

#define POINT_2_VOLTAGE(value, range, rShift)   \
                (((float)(value) - (float)MIN_VALUE) * voltsInPixel[(range)] - MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define TSHIFT_2_ABS(tShift, tBase)  (absStepTShift[(tBase)] * (tShift) * 2.0F)

#define TSHIFT_2_REL(tShiftAbs, tBase) ((tShiftAbs) / absStepTShift[(tBase)] / 2.0F)

#define ROUND(x) (x += 0.5F, x)

#define PI 3.141592653589793F

extern const float tableScalesRange[Range::Count];
extern const float absStepRShift[]; //-V2504
extern const float voltsInPixel[]; //-V2504
extern const float absStepTShift[]; //-V2504

namespace Math
{
    int     MinFrom2Int(int val0, int val1);
    int     RShift2Rel(float rShiftAbs, Range::E range);
    float   VoltageCursor(float shiftCurU, Range::E range, int16 rShift);
    float   TimeCursor(float shiftCurT, TBase::E tBase);
    void    PointsRelToVoltage(pUCHAR points, int numPoints, Range::E range, int16 rShift, float *voltage);
    uint8   VoltageToPoint(float voltage, Range::E range, int16 rShift);
    void	PointsVoltageToRel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);
    uint8   GetMaxFromArrayWithErrorCode(pUCHAR data, int firstPoint, int lastPoint);
    uint8   GetMinFromArrayWithErrorCode(pUCHAR data, int firstPoint, int lastPoint);
    uint8   GetMaxFromArray(pUCHAR data, int firstPoint, int lastPoint);
    uint8   GetMinFromArray(pUCHAR data, int firstPoint, int lastPoint);
    // ¬озвращает координату x пересечени€ линии, проход€щей через (x0, y0), (x1, y1), с горизонтальной линией, проход€щей через точку с ординатой yHorLine.
    float   GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
    // —равнивает два числа. ¬озвращает true, если числа отличаютс€ друг от друга не более, чем на epsilonPart. ѕри этом дл€ расчЄта epsilonPart 
    // используетс€ большее в смысле модул€ число.
    bool    FloatsIsEquals(float value0, float value1, float epsilonPart);
    float   MinFrom3float(float value1, float value2, float value3);
    int     MinInt(int val1, int val2);
}

void    Math_CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);
void    Math_CalculateMathFunction(float *data0andResult, const float *data1, int numPoints);
float   Math_RandFloat(float min, float max);
int8    Math_AddInt8WithLimitation(int8 value, int8 delta, int8 min, int8 max);
int     Math_Sign(int vlaue);
// ¬ычисл€ет 10**pow.
int     Math_Pow10(int pow);
// ¬ычисл€ет число разр€дов в целом типа int.
int     Math_NumDigitsInNumber(int value);
// ¬озвращает модуль value.
int     Math_FabsInt(int value);

int     LimitationInt(int value, int min, int max);
uint8   LimitationUInt8(uint8 value, uint8 min, uint8 max);
float   LimitationFloat(float value, float min, float max);

uint8   Math_CalculateFiltr(pUCHAR data, int x, int numPoints, int numSmoothing);
void    Math_CalculateFiltrArray(pUCHAR dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
