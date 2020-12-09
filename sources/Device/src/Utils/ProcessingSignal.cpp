#include "defines.h"
#include "common/Utils/Math_c.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/ProcessingSignal.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>


struct MeasureValue
{
    float value[2];
};

static uint8 dataOut0[FPGA_MAX_POINTS];
static uint8 dataOut1[FPGA_MAX_POINTS];
static DataSettings *dataSet = 0;
static uint8 dataIn[2][FPGA_MAX_POINTS];

static int firstP = 0;
static int lastP = 0;
static int numP = 0;

const Processing::MeasureCalculate Processing::measures[Measure::Count] =
{
    {0, 0},
    {"CalculateVoltageMax",         CalculateVoltageMax,           Voltage2String, true},
    {"CalculateVoltageMin",         CalculateVoltageMin,           Voltage2String, true},
    {"CalculateVoltagePic",         CalculateVoltagePic,           Voltage2String, false},
    {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,     Voltage2String, true},
    {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,     Voltage2String, true},
    {"CalculateVoltageAmpl",        CalculateVoltageAmpl,          Voltage2String, false},
    {"CalculateVoltageAverage",     CalculateVoltageAverage,       Voltage2String, true},
    {"CalculateVoltageRMS",         CalculateVoltageRMS,           Voltage2String, false},
    {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,    Voltage2String, false},
    {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus,   Voltage2String, false},
    {"CalculatePeriod",             CalculatePeriod,               Time2String, false},
    {"CalculateFreq",               CalculateFreq,                 Freq2String, false},
    {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,      Time2String, false},
    {"CalculateTimeSpada",          CalculateTimeSpada,            Time2String, false},
    {"CalculateDurationPlus",       CalculateDurationPlus,         Time2String, false},
    {"CalculateDurationPlus",       CalculateDurationMinus,        Time2String, false},
    {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,        FloatFract2String, false},
    {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,       FloatFract2String, false},
    {"CalculateDelayPlus",          CalculateDelayPlus,            Time2String, false},
    {"CalculateDelayMinus",         CalculateDelayMinus,           Time2String, false},
    {"CalculatePhazaPlus",          CalculatePhazaPlus,            Phase2String, false},
    {"CalculatePhazaMinus",         CalculatePhazaMinus,           Phase2String, false}
};

static MeasureValue values[Measure::Count] = {{0.0F, 0.0F}};

static int markerHor[NumChannels][2] = {{ERROR_VALUE_INT}, {ERROR_VALUE_INT}};
static int markerVert[NumChannels][2] = {{ERROR_VALUE_INT}, {ERROR_VALUE_INT}};

static bool maxIsCalculating[2] = {false, false};
static bool minIsCalculating[2] = {false, false};
static bool maxSteadyIsCalculating[2] = {false, false};
static bool minSteadyIsCalculating[2] = {false, false};
static bool aveIsCalculating[2] = {false, false};
static bool periodIsCaclulating[2] = {false, false};
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = {false, false};

#define EXIT_IF_ERROR_FLOAT(x)      if(std::fabsf((x) - ERROR_VALUE_FLOAT) < 1e-10F)                                                  { return ERROR_VALUE_FLOAT; }
#define EXIT_IF_ERRORS_FLOAT(x, y)  if(std::fabsf((x) - ERROR_VALUE_FLOAT) < 1e-10F || std::fabsf((y) - ERROR_VALUE_FLOAT) < 1e-10F)  { return ERROR_VALUE_FLOAT; }
#define EXIT_IF_ERROR_INT(x)        if((x) == ERROR_VALUE_INT)                                                                        { return ERROR_VALUE_FLOAT; }

void Processing::CalculateMeasures() //-V2506
{
    if(!SHOW_MEASURES || !dataSet)
    {
        return;
    }

    maxIsCalculating[0] = false;
    maxIsCalculating[1] = false;
    maxSteadyIsCalculating[0] = false;
    maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = false;
    minIsCalculating[1] = false;
    minSteadyIsCalculating[0] = false;
    minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = false;
    aveIsCalculating[1] = false;
    periodIsCaclulating[0] = false;
    periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = false;
    periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = false;
    picIsCalculating[1] = false;

    for(int str = 0; str < Measure::NumRows(); str++)
    {
        for(int elem = 0; elem < Measure::NumCols(); elem++)
        {
            Measure::E meas = Measure::Type(str, elem);
            pFuncFCh func = measures[meas].FuncCalculate;
            if(func)
            {
                if(meas == MEAS_MARKED || MEAS_MARKED_IS_NONE)
                {
                    markerVert[Channel::A][0] = ERROR_VALUE_INT;
                    markerVert[Channel::A][1] = ERROR_VALUE_INT;
                    markerVert[Channel::B][0] = ERROR_VALUE_INT;
                    markerVert[Channel::B][1] = ERROR_VALUE_INT;
                    markerHor[Channel::A][0] = ERROR_VALUE_INT;
                    markerHor[Channel::A][1] = ERROR_VALUE_INT;
                    markerHor[Channel::B][0] = ERROR_VALUE_INT;
                    markerHor[Channel::B][1] = ERROR_VALUE_INT;
                }
                if(MEAS_SOURCE_IS_A || MEAS_SOURCE_IS_A_B)
                {
                    values[meas].value[Channel::A] = func(Channel::A);
                }
                if(MEAS_SOURCE_IS_B || MEAS_SOURCE_IS_A_B)
                {
                    values[meas].value[Channel::B] = func(Channel::B);
                }
            }
        }
    }
}

float Processing::CalculateVoltageMax(Channel::E chan) //-V2506
{
    float max = CalculateMaxRel(chan);
    
    EXIT_IF_ERROR_FLOAT(max); //-V2550 //-V550 //-V2507
    if(MEAS_MARKED == Measure::VoltageMax)
    {
        markerHor[chan][0] = static_cast<int>(max);                           // Здесь не округляем, потому что max может быть только целым
    }

    return POINT_2_VOLTAGE(max, dataSet->range[chan], chan == Channel::A ? dataSet->rShiftCh0 : dataSet->rShiftCh1) * VALUE_MULTIPLIER(chan);
}

float Processing::CalculateVoltageMin(Channel::E chan) //-V2506
{
    float min = CalculateMinRel(chan);
    EXIT_IF_ERROR_FLOAT(min); //-V2550 //-V550
    if(MEAS_MARKED == Measure::VoltageMin)
    {
        markerHor[chan][0] = static_cast<int>(min);                           // Здесь не округляем, потому что min может быть только целым
    }

    return POINT_2_VOLTAGE(min, dataSet->range[chan], chan == Channel::A ? dataSet->rShiftCh0 : dataSet->rShiftCh1) * VALUE_MULTIPLIER(chan);
}

float Processing::CalculateVoltagePic(Channel::E chan) //-V2506
{
    float max = CalculateVoltageMax(chan);
    float min = CalculateVoltageMin(chan);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2550 //-V550

    if(MEAS_MARKED == Measure::VoltagePic)
    {
        markerHor[chan][0] = static_cast<int>(CalculateMaxRel(chan));
        markerHor[chan][1] = static_cast<int>(CalculateMinRel(chan));
    }
    return max - min;
}

float Processing::CalculateVoltageMinSteady(Channel::E chan) //-V2506
{
    float min = CalculateMinSteadyRel(chan);
    EXIT_IF_ERROR_FLOAT(min); //-V2550 //-V550
    if(MEAS_MARKED == Measure::VoltageMinSteady)
    {
        markerHor[chan][0] = static_cast<int>(ROUND(min)); //-V2528
    }

    return (POINT_2_VOLTAGE(min, dataSet->range[chan], chan == Channel::A ? dataSet->rShiftCh0 : dataSet->rShiftCh1) * VALUE_MULTIPLIER(chan));
}

float Processing::CalculateVoltageMaxSteady(Channel::E chan) //-V2506
{
    float max = CalculateMaxSteadyRel(chan);

    EXIT_IF_ERROR_FLOAT(max); //-V2550 //-V550

    if(MEAS_MARKED == Measure::VoltageMaxSteady)
    {
        markerHor[chan][0] = static_cast<int>(max);
    }

    Range::E range = dataSet->range[chan];
    uint rShift = chan == Channel::A ? dataSet->rShiftCh0 : dataSet->rShiftCh1;

    return (POINT_2_VOLTAGE(max, range, rShift) * VALUE_MULTIPLIER(chan));
}

float Processing::CalculateVoltageVybrosPlus(Channel::E chan) //-V2506
{
    float max = CalculateMaxRel(chan);
    float maxSteady = CalculateMaxSteadyRel(chan);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady); //-V2550 //-V550

    if (MEAS_MARKED == Measure::VoltageVybrosPlus)
    {
        markerHor[chan][0] = static_cast<int>(max);
        markerHor[chan][1] = static_cast<int>(maxSteady);
    }

    int16 rShift = chan == Channel::A ? (int16)dataSet->rShiftCh0 : (int16)dataSet->rShiftCh1;
    return std::fabsf(POINT_2_VOLTAGE(maxSteady, dataSet->range[chan], rShift) - POINT_2_VOLTAGE(max, dataSet->range[chan], rShift)) * VALUE_MULTIPLIER(chan);
}

float Processing::CalculateVoltageVybrosMinus(Channel::E chan) //-V2506
{
    float min = CalculateMinRel(chan);
    float minSteady = CalculateMinSteadyRel(chan);
    EXIT_IF_ERRORS_FLOAT(min, minSteady); //-V2550 //-V550

    if (MEAS_MARKED == Measure::VoltageVybrosMinus)
    {
        markerHor[chan][0] = static_cast<int>(min);
        markerHor[chan][1] = static_cast<int>(minSteady);
    }

    int16 rShift = chan == Channel::A ? (int16)dataSet->rShiftCh0 : (int16)dataSet->rShiftCh1;
    return std::fabsf(POINT_2_VOLTAGE(minSteady, dataSet->range[chan], rShift) - POINT_2_VOLTAGE(min, dataSet->range[chan], rShift)) * VALUE_MULTIPLIER(chan);
}

float Processing::CalculateVoltageAmpl(Channel::E chan) //-V2506
{
    float max = CalculateVoltageMaxSteady(chan);
    float min = CalculateVoltageMinSteady(chan);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2550 //-V550

    if(MEAS_MARKED == Measure::VoltageAmpl)
    {
        markerHor[chan][0] = static_cast<int>(CalculateMaxSteadyRel(chan));
        markerHor[chan][1] = static_cast<int>(CalculateMinSteadyRel(chan));
    }
    return max - min;
}

float Processing::CalculateVoltageAverage(Channel::E chan) //-V2506
{
    int period = CalculatePeriodAccurately(chan);

    EXIT_IF_ERROR_INT(period);

    int sum = 0;
    uint8 *data = &dataIn[chan][firstP];
    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = static_cast<uint8>((float)sum / period);

    if(MEAS_MARKED == Measure::VoltageAverage)
    {
        markerHor[chan][0] = aveRel;
    }

    return (POINT_2_VOLTAGE(aveRel, dataSet->range[chan], chan == Channel::A ? dataSet->rShiftCh0 : dataSet->rShiftCh1) * VALUE_MULTIPLIER(chan));
}

float Processing::CalculateVoltageRMS(Channel::E chan) //-V2506
{
    int period = CalculatePeriodAccurately(chan);

    EXIT_IF_ERROR_INT(period);

    float rms = 0.0F;
    int16 rShift = chan == Channel::A ? (int16)dataSet->rShiftCh0 : (int16)dataSet->rShiftCh1;
    for(int i = firstP; i < firstP + period; i++)
    {
        float volts = POINT_2_VOLTAGE(dataIn[chan][i], dataSet->range[chan], rShift);
        rms +=  volts * volts;
    }

    if(MEAS_MARKED == Measure::VoltageRMS)
    {
        markerHor[chan][0] = MathFPGA::VoltageToPoint(std::sqrtf(rms / period), dataSet->range[chan], rShift);
    }

    return std::sqrtf(rms / period) * VALUE_MULTIPLIER(chan);
}

float Processing::CalculatePeriod(Channel::E chan) //-V2506
{
    static float period[2] = {0.0F, 0.0F};

    if(!periodIsCaclulating[chan])
    {
        float aveValue = CalculateAverageRel(chan);
        if(aveValue == ERROR_VALUE_UINT8) //-V2550 //-V550
        {
            period[chan] = ERROR_VALUE_FLOAT;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(chan, 1, true, static_cast<uint8>(aveValue));
            float intersectionTopToDown = FindIntersectionWithHorLine(chan, 1, false, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown); //-V2550 //-V550

            float firstIntersection = intersectionDownToTop < intersectionTopToDown ? intersectionDownToTop : intersectionTopToDown;
            float secondIntersection = FindIntersectionWithHorLine(chan, 2, intersectionDownToTop < intersectionTopToDown, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2550 //-V550

            float per = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);

            period[chan] = per;
            periodIsCaclulating[chan] = true;
        }
    }

    return period[chan];
}

#define EXIT_FROM_PERIOD_ACCURACY               \
    period[chan] = ERROR_VALUE_INT;             \
    periodAccurateIsCalculating[chan] = true;   \
    return period[chan];

int Processing::CalculatePeriodAccurately(Channel::E chan) //-V2506
{
    static int period[2];

    int sums[FPGA_MAX_POINTS];

    if(!periodAccurateIsCalculating[chan])
    {
        period[chan] = 0;
        float pic = CalculatePicRel(chan);

        if(pic == ERROR_VALUE_FLOAT) //-V2550 //-V550
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = static_cast<int>(pic * 5);
        sums[firstP] = dataIn[chan][firstP];

        int i = firstP + 1;
        int *sum = &sums[i];
        uint8 *data = &dataIn[chan][i];
        uint8 *end = &dataIn[chan][lastP];

        while (data < end)
        {
            uint8 point = *data++;
            if(point < MIN_VALUE || point >= MAX_VALUE)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *sum = *(sum - 1) + point;
            sum++;
        }

        int addShift = firstP - 1;
        int maxPeriod = static_cast<int>(numP * 0.95F);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int s = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = numP - nextPeriod;

            int *pSums = &sums[firstP + 1];
            for(int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - s;
                if (nextSum < s)
                {
                    nextDelta = -nextDelta;
                }

                if(nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if(nextDelta > maxDelta)
                {
                    maxDelta = nextDelta;
                }
            }

            if(maxDelta < delta)
            {
                delta = maxDelta;
                period[chan] = nextPeriod;
            }
        }

        if(period[chan] == 0)
        {
            period[chan] = ERROR_VALUE_INT;
        }
        periodAccurateIsCalculating[chan] = true;
    }

    return period[chan];
}

float Processing::CalculateFreq(Channel::E chan)
{
    float period = CalculatePeriod(chan);
    return period == ERROR_VALUE_FLOAT ? ERROR_VALUE_FLOAT : 1.0F / period; //-V2550 //-V550
}

float Processing::FindIntersectionWithHorLine(Channel::E chan, int numIntersection, bool downToUp, uint8 yLine) //-V2506
{
    int num = 0;
    int x = firstP;
    int compValue = lastP - 1;

    uint8 *data = &dataIn[chan][0];

    if(downToUp)
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] < yLine && data[x + 1] >= yLine)
            {
                num++;
            }
            x++;
        }
    }
    else
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] > yLine && data[x + 1] <= yLine)
            {
                num++;
            }
            x++;
        }
    }
    x--;

    if (num < numIntersection)
    {
        return ERROR_VALUE_FLOAT;
    }
    return Math::GetIntersectionWithHorizontalLine(x, data[x], x + 1, data[x + 1], yLine);
}

float Processing::CalculateDurationPlus(Channel::E chan) //-V2506
{
    float aveValue = CalculateAverageRel(chan);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2550 //-V550

    float firstIntersection = FindIntersectionWithHorLine(chan, 1, true, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(chan, 1, false, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(chan, 2, false, static_cast<uint8>(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);
}

float Processing::CalculateDurationMinus(Channel::E chan) //-V2506
{
    float aveValue = CalculateAverageRel(chan);
    EXIT_IF_ERROR_FLOAT(aveValue);

    float firstIntersection = FindIntersectionWithHorLine(chan, 1, false, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(chan, 1, true, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(chan, 2, true, static_cast<uint8>(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);
}

float Processing::CalculateTimeNarastaniya(Channel::E chan)                    // WARN Здесь, возможно, нужно увеличить точность - брать не целые значени расстояний между отсчётами по времени, а рассчитывать пересечения линий //-V2506
{
    float maxSteady = CalculateMaxSteadyRel(chan);
    float minSteady = CalculateMinSteadyRel(chan);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(chan, 1, true, static_cast<uint8>(min01));
    float secondIntersection = FindIntersectionWithHorLine(chan, 1, true, static_cast<uint8>(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(chan, 2, true, static_cast<uint8>(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);

    if (MEAS_MARKED == Measure::TimeNarastaniya)
    {
        markerHor[chan][0] = static_cast<int>(max09);
        markerHor[chan][1] = static_cast<int>(min01);
        markerVert[chan][0] = static_cast<int>(firstIntersection - SHIFT_IN_MEMORY);
        markerVert[chan][1] = static_cast<int>(secondIntersection - SHIFT_IN_MEMORY);
    }

    return retValue;
}

float Processing::CalculateTimeSpada(Channel::E chan)                          // WARN Аналогично времени нарастания //-V2506
{
    float maxSteady = CalculateMaxSteadyRel(chan);
    float minSteady = CalculateMinSteadyRel(chan);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(chan, 1, false, static_cast<uint8>(max09));
    float secondIntersection = FindIntersectionWithHorLine(chan, 1, false, static_cast<uint8>(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(chan, 2, false, static_cast<uint8>(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);

    if (MEAS_MARKED == Measure::TimeSpada)
    {
        markerHor[chan][0] = static_cast<int>(max09);
        markerHor[chan][1] = static_cast<int>(min01);
        markerVert[chan][0] = static_cast<int>(firstIntersection - SHIFT_IN_MEMORY);
        markerVert[chan][1] = static_cast<int>(secondIntersection - SHIFT_IN_MEMORY);
    }

    return retValue;
}

float Processing::CalculateSkvaznostPlus(Channel::E chan) //-V2506
{
    float period = CalculatePeriod(chan);
    float duration = CalculateDurationPlus(chan);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}

float Processing::CalculateSkvaznostMinus(Channel::E chan) //-V2506
{
    float period = CalculatePeriod(chan);
    float duration = CalculateDurationMinus(chan);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}

float Processing::CalculateMinSteadyRel(Channel::E chan)
{
    static float min[2] = {255.0F, 255.0F};

    if(!minSteadyIsCalculating[chan])
    {
        float aveValue = CalculateAverageRel(chan);
        if(aveValue == ERROR_VALUE_FLOAT) //-V2550 //-V550
        {
            min[chan] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[chan][firstP];
            pUCHAR const end = &dataIn[chan][lastP];
            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            min[chan] = (float)sum / numSums;
            int numMin = numSums; //-V2551

            float pic = CalculatePicRel(chan);
            if (pic == ERROR_VALUE_FLOAT) //-V2550 //-V550
            {
                min[chan] = ERROR_VALUE_FLOAT;
            }
            else
            {
                int numDeleted = 0;
                float value = pic / 9.0F;
                data = &dataIn[chan][firstP];
                float _min = min[chan];
                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d < aveValue)
                    {
                        if (d < _min)
                        {
                            if (_min - d > value)
                            {
                                sum -= d;
                                --numSums;
                                ++numDeleted;
                            }
                        }
                        else if (d - _min > value)
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted;
                        }
                    }
                }
                min[chan] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(chan) : (float)sum / numSums;
            }
        }
        minSteadyIsCalculating[chan] = true;
    }

    return min[chan];
}

float Processing::CalculateMaxSteadyRel(Channel::E chan)
{
    static float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[chan])
    {
        float aveValue = CalculateAverageRel(chan);
        
        if(aveValue == ERROR_VALUE_FLOAT) //-V2550 //-V550
        {
            max[chan] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[chan][firstP];
            pUCHAR const end = &dataIn[chan][lastP];
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            max[chan] = (float)sum / numSums;

            float pic = CalculatePicRel(chan);

            if (pic == ERROR_VALUE_FLOAT) //-V2550 //-V550
            {
                max[chan] = ERROR_VALUE_FLOAT;
            }
            else
            {
                int numDeleted = 0;
                int numMax = numSums;
                float value = pic / 9.0F;

                data = &dataIn[chan][firstP];
                uint8 _max = static_cast<uint8>(max[chan]);
                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d > aveValue)
                    {
                        if (d > _max)
                        {
                            if (d - _max > value)
                            {
                                sum -= d;
                                numSums--;
                                numDeleted++;
                            }
                        }
                        else if (_max - d > value)
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++;
                        }
                    }
                }
                max[chan] = (numDeleted > numMax / 2) ? CalculateMaxRel(chan) : (float)sum / numSums;
            }
        }
        maxSteadyIsCalculating[chan] = true;
    }

    return max[chan];
}

float Processing::CalculateMaxRel(Channel::E chan)
{
    static float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[chan])
    {
        uint8 val = MathFPGA::GetMaxFromArrayWithErrorCode(dataIn[chan], firstP, lastP);
        max[chan] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        maxIsCalculating[chan] = true;
    }

    return max[chan];
}

float Processing::CalculateMinRel(Channel::E chan)
{
    static float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[chan])
    {
        uint8 val = MathFPGA::GetMinFromArrayWithErrorCode(dataIn[chan], firstP, lastP);
        min[chan] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        minIsCalculating[chan] = true;
    }

    return min[chan];
}

float Processing::CalculateAverageRel(Channel::E chan)
{
    static float ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[chan])
    {
        float min = CalculateMinRel(chan);
        float max = CalculateMaxRel(chan);
        ave[chan] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : (min + max) / 2.0F; //-V2550 //-V550
        aveIsCalculating[chan] = true;
    }
    return ave[chan];
}

float Processing::CalculatePicRel(Channel::E chan)
{
    static float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[chan])
    {
        float min = CalculateMinRel(chan);
        float max = CalculateMaxRel(chan);
        pic[chan] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : max - min; //-V2550 //-V550
        picIsCalculating[chan] = true;
    }
    return pic[chan];
}

float Processing::CalculateDelayPlus(Channel::E chan) //-V2506
{
    float period0 = CalculatePeriod(Channel::A);
    float period1 = CalculatePeriod(Channel::B);

    EXIT_IF_ERRORS_FLOAT(period0, period1);
    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return ERROR_VALUE_FLOAT;
    }

    float average0 = CalculateAverageRel(Channel::A);
    float average1 = CalculateAverageRel(Channel::B);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = (chan == Channel::A) ? average0 : average1;
    float averageSecond = (chan == Channel::A) ? average1 : average0;
    Channel::E firstChannel = (chan == Channel::A) ? Channel::A : Channel::B;
    Channel::E secondChannel = (chan == Channel::A) ? Channel::B : Channel::A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, static_cast<uint8>(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, static_cast<uint8>(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, static_cast<uint8>(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);
}

float Processing::CalculateDelayMinus(Channel::E chan) //-V2506
{
    float period0 = CalculatePeriod(Channel::A);
    float period1 = CalculatePeriod(Channel::B);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return ERROR_VALUE_FLOAT;
    }

    float average0 = CalculateAverageRel(Channel::A);
    float average1 = CalculateAverageRel(Channel::B);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = (chan == Channel::A) ? average0 : average1;
    float averageSecond = (chan == Channel::A) ? average1 : average0;
    Channel::E firstChannel = (chan == Channel::A) ? Channel::A : Channel::B;
    Channel::E secondChannel = (chan == Channel::A) ? Channel::B : Channel::A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, static_cast<uint8>(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, static_cast<uint8>(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, static_cast<uint8>(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);
}

float Processing::CalculatePhazaPlus(Channel::E chan) //-V2506
{
    float delay = CalculateDelayPlus(chan);
    float period = CalculatePeriod(chan);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT) //-V2550 //-V550
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0F;
}

float Processing::CalculatePhazaMinus(Channel::E chan) //-V2506
{
    float delay = CalculateDelayMinus(chan);
    float period = CalculatePeriod(chan);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT) //-V2550 //-V550
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0F; 
}

void Processing::SetSignal(pUCHAR data0, pUCHAR data1, DataSettings *ds, int _firstPoint, int _lastPoint) //-V2506
{
    firstP = _firstPoint;
    lastP = _lastPoint;
    numP = lastP - firstP;
    dataSet = ds;

    if (ds == nullptr)
    {
        return;
    }
    
    int numSmoothing = Smoothing::NumPoints();

    int length = (int)ds->length1channel * (ds->peakDet == PeackDetMode::Disable ? 1 : 2);

    Math::CalculateFiltrArray(data0, &dataIn[Channel::A][0], length, numSmoothing);
    Math::CalculateFiltrArray(data1, &dataIn[Channel::B][0], length, numSmoothing);

    CountedToCurrentSettings();
}

void Processing::GetData(uint8 **data0, uint8 **data1, DataSettings **ds)
{
    if (data0)
    {
        *data0 = dataOut0;
    }
    if (data1)
    {
        *data1 = dataOut1;
    }
    *ds = dataSet;
}

float Processing::GetCursU(Channel::E chan, float posCurT)
{   
    int first = 0;
    int last = 0;
    SettingsDisplay::PointsOnDisplay(&first, &last);

    float retValue;
    LIMITATION(retValue, static_cast<float>(200.0F - (dataIn[chan])[first + (int)posCurT] + MIN_VALUE), 0.0F, 200.0F);
    return retValue;
}

float Processing::GetCursT(Channel::E chan, float posCurU, int numCur) //-V2506
{
    int firstPoint = 0;
    int lastPoint = 0;
    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    int prevData = 200 - (dataIn[chan])[firstPoint] + MIN_VALUE;

    int numIntersections = 0;

    for(int i = firstPoint + 1; i < lastPoint; i++)
    {
        int curData = 200 - (dataIn[chan])[i] + MIN_VALUE;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return static_cast<float>(i - firstPoint);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return static_cast<float>(i - firstPoint);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return static_cast<float>(i - firstPoint);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return static_cast<float>(i - firstPoint);
                }
            }
        }
        prevData = curData;
    }
    return 0;
}

void Processing::InterpolationSinX_X(uint8 data[FPGA_MAX_POINTS], TBase::E tBase)
{
/*
     Последовательности x в sin(x)
2    1. 50нс : pi/2, -pi/2 ...
8    2. 20нс : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 10нс : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 5нс  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 2нс  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    int deltas[5] = {50, 20, 10, 5, 2};
    int delta = deltas[tBase];

    uint8 signedData[FPGA_MAX_POINTS / 2];
    int numSignedPoints = 0;
    
    for (int pos = 0; pos < FPGA_MAX_POINTS; pos++)
    {
        if (data[pos] > 0)
        {
            signedData[numSignedPoints] = data[pos];
            numSignedPoints++;
        }
    }

    // Найдём смещение первой значащей точки

    int shift = 0;
    for (int pos = 0; pos < FPGA_MAX_POINTS; pos++)
    {
        if (data[pos] > 0)
        {
            shift = pos;
            break;
        }
    }

    float deltaX = PI;
    float stepX0 = PI / (float)delta;
    float x0 = PI - stepX0;
    int num = 0;
    
    for(int i = 0; i < FPGA_MAX_POINTS; i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            data[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? std::sin(PI / delta * (part + 1)) : std::sin(PI / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)                 // Здесь используем более быструю, но более неправильную арифметику целвых чисел
            {
                int sinXint = (int)(sinX * MUL_SIN);
                int value = 0;
                int x = (int)((x0 - deltaX) * MUL);
                int deltaXint = (int)(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF);
            }
            else                                    // На этих развёртках арифметика с плавающей запятой даёт приемлемое быстродействие
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }
                data[i] = (uint8)value;
            }
        }
    }
    
    int pos = FPGA_MAX_POINTS - 1;
    while (pos > shift)
    {
        data[pos] = data[pos - shift];
        pos--;
    }
}

char* Processing::GetStringMeasure(Measure::E measure, Channel::E chan, char buffer[20]) //-V2506
{
    if (!SET_ENABLED(chan))
    {
        return "";
    }
    buffer[0] = '\0';
    std::sprintf(buffer, chan == Channel::A ? "1: " : "2: ");
    if(dataSet == 0)
    {
        std::strcat(buffer, "-.-"); //-V2513
    }
    else if((chan == Channel::A && dataSet->enableCh0 == 0) || (chan == Channel::B && dataSet->enableCh1 == 0))
    {
    }
    else if(measures[measure].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncCFBC func = measures[measure].FucnConvertate;
        float value = values[measure].value[chan];
        char *text = func(value, measures[measure].showSign, bufferForFunc);
        std::strcat(buffer, text); //-V2513
    }
    else
    {
        return buffer;
    }
    return buffer;
}

int Processing::GetMarkerHorizontal(Channel::E chan, int numMarker)
{
    return markerHor[chan][numMarker] - MIN_VALUE;
}

int Processing::GetMarkerVertical(Channel::E chan, int numMarker)
{
    return markerVert[chan][numMarker];
}

void Processing::CountedToCurrentSettings()
{
    std::memset(dataOut0, 0, FPGA_MAX_POINTS);
    std::memset(dataOut1, 0, FPGA_MAX_POINTS);
    
    int numPoints = (int)dataSet->length1channel * (dataSet->peakDet == PeackDetMode::Disable ? 1 : 2);

    int16 dataTShift = dataSet->tShift;
    int16 curTShift = TSHIFT;

    int16 dTShift = curTShift - dataTShift;
    for (int i = 0; i < numPoints; i++)
    {
        int index = i - dTShift;
        if (index >= 0 && index < numPoints)
        {
            dataOut0[index] = dataIn[0][i];
            dataOut1[index] = dataIn[1][i];
        }
    }
 
    if (dataSet->enableCh0 == 1U && (dataSet->range[0] != SET_RANGE_A || dataSet->rShiftCh0 != (uint)SET_RSHIFT_A))
    {
        Range::E range = SET_RANGE_A;
        int16 rShift = SET_RSHIFT_A;

        for (int i = 0; i < numPoints; i++)
        {
            float absValue = POINT_2_VOLTAGE(dataOut0[i], dataSet->range[0], dataSet->rShiftCh0);
            int relValue = static_cast<int>((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) / MathFPGA::voltsInPixel[range] + MIN_VALUE);

            if (relValue < MIN_VALUE)       { dataOut0[i] = MIN_VALUE; }
            else if (relValue > MAX_VALUE)  { dataOut0[i] = MAX_VALUE; }
            else                            { dataOut0[i] = (uint8)relValue; }
        }
    }
    if (dataSet->enableCh1 == 1 && (dataSet->range[1] != SET_RANGE_B || dataSet->rShiftCh1 != (uint)SET_RSHIFT_B))
    {
        Range::E range = SET_RANGE_B;
        int16 rShift = SET_RSHIFT_B;

        for (int i = 0; i < numPoints; i++)
        {
            float absValue = POINT_2_VOLTAGE(dataOut1[i], dataSet->range[1], dataSet->rShiftCh1);
            int relValue = static_cast<int>((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) / MathFPGA::voltsInPixel[range] + MIN_VALUE);

            if (relValue < MIN_VALUE)       { dataOut1[i] = MIN_VALUE; }
            else if (relValue > MAX_VALUE)  { dataOut1[i] = MAX_VALUE; }
            else                            { dataOut1[i] = (uint8)relValue; }
        }
    }
}
