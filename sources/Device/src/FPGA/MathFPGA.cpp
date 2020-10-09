#include "defines.h"
#include "common/Utils/Math.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include <cmath>


const float tableScalesRange[Range::Count] = { 2e-3F, 5e-3F, 10e-3F, 20e-3F, 50e-3F, 100e-3F, 200e-3F, 500e-3F, 1.0F, 2.0F, 5.0F, 10.0F, 20.0F };
/*
static const float tableScalesTBase[TBase::Count] =
    {2e-9, 5e-9, 10e-9, 20e-9, 50e-9, 100e-9, 200e-9, 500e-9,
    1e-6, 2e-6, 5e-6, 10e-6, 20e-6, 50e-6, 100e-6, 200e-6, 500e-6,
    1e-3, 2e-3, 5e-3, 10e-3, 20e-3, 50e-3, 100e-3, 200e-3, 500e-3,
    1.0F, 2.0F, 5.0F, 10.0F};
*/


const int voltsInPixelInt[] =   // Коэффициент 20000
{
    2,      // 2
    5,      // 5
    10,     // 10
    20,     // 20
    50,     // 50
    100,    // 100
    200,    // 200
    500,    // 500
    1000,   // 1
    2000,   // 2
    5000,   // 5
    1000,   // 10
    20000   // 20
};


const float absStepRShift[] =
{
    2e-3F / 20 / STEP_RSHIFT,
    5e-3F / 20 / STEP_RSHIFT,
    10e-3F / 20 / STEP_RSHIFT,
    20e-3F / 20 / STEP_RSHIFT,
    50e-3F / 20 / STEP_RSHIFT,
    100e-3F / 20 / STEP_RSHIFT,
    200e-3F / 20 / STEP_RSHIFT,
    500e-3F / 20 / STEP_RSHIFT,
    1.0F / 20 / STEP_RSHIFT,
    2.0F / 20 / STEP_RSHIFT,
    5.0F / 20 / STEP_RSHIFT,
    10.0F / 20 / STEP_RSHIFT,
    20.0F / 20 / STEP_RSHIFT
};


const float voltsInPixel[] =
{
    2e-3F / 20,      // 2mV
    5e-3F / 20,      // 5mV
    10e-3F / 20,     // 10mV
    20e-3F / 20,     // 20mV
    50e-3F / 20,     // 50mV
    100e-3F / 20,    // 100mV
    200e-3F / 20,    // 200mV
    500e-3F / 20,    // 500mV
    1.0F / 20,       // 1V
    2.0F / 20,       // 2V
    5.0F / 20,       // 5V
    10.0F / 20,      // 10V
    20.0F / 20       // 20V
};

const float absStepTShift[] =
{
    2e-9F / 20, 5e-9F / 20, 10e-9F / 20, 20e-9F / 20, 50e-9F / 20, 100e-9F / 20, 200e-9F / 20, 500e-9F / 20,
    1e-6F / 20, 2e-6F / 20, 5e-6F / 20, 10e-6F / 20, 20e-6F / 20,   50e-6F / 20, 100e-6F / 20, 200e-6F / 20, 500e-6F / 20,
    1e-3F / 20, 2e-3F / 20, 5e-3F / 20, 10e-3F / 20, 20e-3F / 20,   50e-3F / 20, 100e-3F / 20, 200e-3F / 20, 500e-3F / 20,
    1.0F / 20, 2.0F / 20, 5.0F / 20, 10.0F / 20
};


int Math::RShift2Rel(float rShiftAbs, Range::E range)
{
    int retValue = static_cast<int>(RShiftZero + rShiftAbs / absStepRShift[range]);
    if (retValue < RShiftMin)
    {
        retValue = RShiftMin;
    }
    else if (retValue > RShiftMax)
    {
        retValue = RShiftMax;
    }
    return retValue;
};


float Math::VoltageCursor(float shiftCurU, Range::E range, int16 rShift)
{
    return MAX_VOLTAGE_ON_SCREEN(range) - shiftCurU * voltsInPixel[range] - RSHIFT_2_ABS(rShift, range);
}


void Math::PointsRelToVoltage(pUCHAR points, int numPoints, Range::E range, int16 rShift, float* voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    int diff = static_cast<int>((MIN_VALUE * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3F);
    float koeff = 1.0F / 20e3F;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = static_cast<float>(points[i] * voltInPixel - diff) * koeff;
    }
}


void Math::PointsVoltageToRel(const float* voltage, int numPoints, Range::E range, int16 rShift, uint8* points)
{
    float maxVoltOnScreen = MAX_VOLTAGE_ON_SCREEN(range);
    float rShiftAbs = RSHIFT_2_ABS(rShift, range);
    float voltInPixel = 1.0F / voltsInPixel[range];

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + MIN_VALUE;

    for (int i = 0; i < numPoints; i++)
    {
        int value = static_cast<int>(voltage[i] * voltInPixel + delta);
        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }
        points[i] = (uint8)value;
    }
}


uint8 Math::VoltageToPoint(float voltage, Range::E range, int16 rShift)
{
    int relValue = static_cast<int>((voltage + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) / voltsInPixel[range] + MIN_VALUE);
    LIMITATION(relValue, relValue, 0, 255);
    return (uint8)relValue;
}


float Math::TimeCursor(float shiftCurT, TBase::E tBase)
{
    return shiftCurT * absStepTShift[tBase];
}


/*
    Быстрое преобразование Фурье. Вычисляет модуль спектра для дейсвтительного сигнала.
    Количество отсчётов должно быть 2**N
*/

#ifndef DEBUG
#include "TablesWindow.h"
#include "TablesLog.h"

static float const* Koeff(int numPoints)
{
    float const* tables[3][4] = {
        {koeffsNorm256, koeffsHamming256, koeffsBlack256, koeffsHann256},
        {koeffsNorm512, koeffsHamming512, koeffsBlack512, koeffsHann512},
        {koeffsNorm1024, koeffsHamming1024, koeffsBlack1024, koeffsHann1024},
    };

    int row = 0;
    if (numPoints == 512)
    {
        row = 1;
    }
    else if (numPoints == 1024)
    {
        row = 2;
    }

    return tables[row][PageServiceMath_GetWindowFFT()];
}

#endif


static void MultiplyToWindow(float* data, int numPoints)
{
#ifndef DEBUG
    float const* koeff = Koeff(numPoints);

    for (int i = 0; i < numPoints; i++)
    {
        data[i] *= koeff[i];
    }
#else
    if (WINDOW_FFT_IS_HAMMING)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.53836F - 0.46164F * std::cos(2 * 3.1415926F * i / (numPoints - 1));
        }
    }
    else if (WINDOW_FFT_IS_BLACKMAN)
    {
        float alpha = 0.16F;
        float a0 = (1.0F - alpha) / 2.0F;
        float a1 = 0.5F;
        float a2 = alpha / 2.0F;
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= a0 - a1 * std::cos(2 * 3.1415926F * i / (numPoints - 1)) + a2 * std::cos(4 * 3.1415926F * i / (numPoints - 1));
        }
    }
    else if (WINDOW_FFT_IS_HANN)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.5F * (1.0F - std::cos(2.0F * 3.1415926F * i / (numPoints - 1.0F)));
        }
    }
#endif
}


static void Normalize(float* data, int)
{
    float max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    for (int i = 0; i < 256; i++)
    {
        data[i] /= max;
    }
}


void Math::CalculateFFT(float* dataR, int numPoints, float* result, float* freq0, float* density0, float* freq1, float* density1, int* y0, int* y1)
{
    float scale = 1.0F / absStepTShift[SET_TBASE] / 1024.0F;

    float K = 1024.0F / numPoints;

    *freq0 = scale * FFT_POS_CURSOR_0 * K;
    *freq1 = scale * FFT_POS_CURSOR_1 * K;
    if (PEAKDET)
    {
        *freq0 *= 2;
        *freq1 *= 2;
    }

    for (int i = 0; i < numPoints; i++)
    {
        result[i] = 0.0F;
    }

    MultiplyToWindow(dataR, numPoints);

    int logN = 8;
    if (numPoints == 512)
    {
        logN = 9;
    }
    else if (numPoints == 1024)
    {
        logN = 10;
    }
    static const float Rcoef[14] =
    {
        -1.0000000000000000F, 0.0000000000000000F, 0.7071067811865475F,
        0.9238795325112867F, 0.9807852804032304F, 0.9951847266721969F,
        0.9987954562051724F, 0.9996988186962042F, 0.9999247018391445F,
        0.9999811752826011F, 0.9999952938095761F, 0.9999988234517018F,
        0.9999997058628822F, 0.9999999264657178F
    };

    static const float Icoef[14] =
    {
        0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    int nn = numPoints >> 1;
    int ie = numPoints;

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0F;
        float iu = 0.0F;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < numPoints; i += ie)
            {
                int io = i + in;
                float dRi = dataR[i];
                float dRio = dataR[io];
                float ri = result[i];
                float rio = result[io];
                dataR[i] = dRi + dRio;
                result[i] = ri + rio;
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu;
                result[io] = itq * ru + rtq * iu;
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < numPoints; i++)
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in];
            float itp = result[in];
            dataR[in] = dataR[io];
            result[in] = result[io];
            dataR[io] = rtp;
            result[io] = itp;
        }

        int k = nn;

        while (k < j)
        {
            j = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    for (int i = 0; i < 256; i++)
    {
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]);
    }

    result[0] = 0.0F;       // WARN нулевая составляющая мешает постоянно. надо её убрать

    Normalize(result, 256);

    if (SCALE_FFT_IS_LOG)
    {
        float minDB = sService_MathGetFFTmaxDBabs();

        for (int i = 0; i < 256; i++)
        {
#ifdef DEBUG
            result[i] = 20 * std::log10f(result[i]);
#else
            result[i] = Log10[(int)(result[i] * 10000)];
#endif
            if (i == FFT_POS_CURSOR_0)
            {
                *density0 = result[i];
            }
            else if (i == FFT_POS_CURSOR_1)
            {
                *density1 = result[i];
            }
            if (result[i] < minDB)
            {
                result[i] = minDB;
            }
            result[i] = 1.0F - result[i] / minDB;
        }
    }
    else
    {
        *density0 = result[FFT_POS_CURSOR_0];
        *density1 = result[FFT_POS_CURSOR_1];
    }
    *y0 = static_cast<int>(Grid::MathBottom() - result[FFT_POS_CURSOR_0] * Grid::MathHeight());
    *y1 = static_cast<int>(Grid::MathBottom() - result[FFT_POS_CURSOR_1] * Grid::MathHeight());
}


uint8 Math::GetMaxFromArrayWithErrorCode(pUCHAR data, int firstPoint, int lastPoint)
{
    uint8 max = Math::GetMaxFromArray(data, firstPoint, lastPoint);
    if (max >= MAX_VALUE)
    {
        max = ERROR_VALUE_UINT8;
    }
    return max;
}


uint8 Math::GetMinFromArrayWithErrorCode(pUCHAR data, int firstPoint, int lastPoint)
{
    uint8 min = Math::GetMinFromArray(data, firstPoint, lastPoint);
    if (min < MIN_VALUE || min >= MAX_VALUE)
    {
        min = ERROR_VALUE_UINT8;
    }
    return min;
}


void Math::CalculateMathFunction(float* data0andResult, const float* data1, int numPoints)
{
    if (MATH_FUNC_IS_SUM)
    {
        int delta = data1 - data0andResult;
        float* end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult += *(data0andResult + delta);
            data0andResult++;
        }
    }
    else if (MATH_FUNC_IS_MUL)
    {
        int delta = data1 - data0andResult;
        float* end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult *= *(data0andResult + delta);
            data0andResult++;
        }
    }
}
