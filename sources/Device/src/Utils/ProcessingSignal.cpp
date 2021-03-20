#include "defines.h"
#include "common/Utils/GlobalFunctions_.h"
#include "common/Utils/Math_.h"
#include "FPGA/DataSettings.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/MathFPGA.h"
#include "Settings/Settings.h"
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

static uint firstP = 0;
static uint lastP = 0;
static uint numP = 0;


static char *FloatFract2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, GF::FloatFract2String(value, always_sign).c_str());
}

static char *Voltage2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, GF::Voltage2String(value, always_sign).c_str());
}

static char *Time2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, GF::Time2String(value, always_sign).c_str());
}

static char *Freq2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, GF::Freq2String(value, always_sign).c_str());
}

static char *Phase2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, GF::Phase2String(value, always_sign).c_str());
}


const Processing::MeasureCalculate Processing::measures[Measure::Count] =
{
    {0, 0},
    {"CalculateVoltageMax",         CalculateVoltageMax,           Voltage2StringC, true},
    {"CalculateVoltageMin",         CalculateVoltageMin,           Voltage2StringC, true},
    {"CalculateVoltagePic",         CalculateVoltagePic,           Voltage2StringC, false},
    {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,     Voltage2StringC, true},
    {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,     Voltage2StringC, true},
    {"CalculateVoltageAmpl",        CalculateVoltageAmpl,          Voltage2StringC, false},
    {"CalculateVoltageAverage",     CalculateVoltageAverage,       Voltage2StringC, true},
    {"CalculateVoltageRMS",         CalculateVoltageRMS,           Voltage2StringC, false},
    {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,    Voltage2StringC, false},
    {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus,   Voltage2StringC, false},
    {"CalculatePeriod",             CalculatePeriod,               Time2StringC, false},
    {"CalculateFreq",               CalculateFreq,                 Freq2StringC, false},
    {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,      Time2StringC, false},
    {"CalculateTimeSpada",          CalculateTimeSpada,            Time2StringC, false},
    {"CalculateDurationPlus",       CalculateDurationPlus,         Time2StringC, false},
    {"CalculateDurationPlus",       CalculateDurationMinus,        Time2StringC, false},
    {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,        FloatFract2StringC, false},
    {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,       FloatFract2StringC, false},
    {"CalculateDelayPlus",          CalculateDelayPlus,            Time2StringC, false},
    {"CalculateDelayMinus",         CalculateDelayMinus,           Time2StringC, false},
    {"CalculatePhazaPlus",          CalculatePhazaPlus,            Phase2StringC, false},
    {"CalculatePhazaMinus",         CalculatePhazaMinus,           Phase2StringC, false}
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

#define EXIT_IF_ERROR_FLOAT(x)      if(std::fabsf((x) - ERROR_VALUE_FLOAT) < 1e-10F)    \
                                        { return ERROR_VALUE_FLOAT; }

#define EXIT_IF_ERRORS_FLOAT(x, y)  if(std::fabsf((x) - ERROR_VALUE_FLOAT) < 1e-10F ||                                 \
                                        std::fabsf((y) - ERROR_VALUE_FLOAT) < 1e-10F)  { return ERROR_VALUE_FLOAT; }

#define EXIT_IF_ERROR_INT(x)        if((x) == ERROR_VALUE_INT)      \
                                        { return ERROR_VALUE_FLOAT; }

void Processing::CalculateMeasures()
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
                    markerVert[ChA][0] = ERROR_VALUE_INT;
                    markerVert[ChA][1] = ERROR_VALUE_INT;
                    markerVert[ChB][0] = ERROR_VALUE_INT;
                    markerVert[ChB][1] = ERROR_VALUE_INT;
                    markerHor[ChA][0] = ERROR_VALUE_INT;
                    markerHor[ChA][1] = ERROR_VALUE_INT;
                    markerHor[ChB][0] = ERROR_VALUE_INT;
                    markerHor[ChB][1] = ERROR_VALUE_INT;
                }
                if(MEAS_SOURCE_IS_A || MEAS_SOURCE_IS_A_B)
                {
                    values[meas].value[ChA] = func(ChA.value);
                }
                if(MEAS_SOURCE_IS_B || MEAS_SOURCE_IS_A_B)
                {
                    values[meas].value[ChB] = func(ChB.value);
                }
            }
        }
    }
}

float Processing::CalculateVoltageMax(Channel::E ch)
{
    float max = CalculateMaxRel(ch);
    
    EXIT_IF_ERROR_FLOAT(max); //-V2507
    if(MEAS_MARKED == Measure::VoltageMax)
    {
        markerHor[ch][0] = static_cast<int>(max);         // Здесь не округляем, потому что max может быть только целым
    }

    return POINT_2_VOLTAGE(max, dataSet->range[ch], (ch == ChA) ? dataSet->r_shift_a : dataSet->rShiftCh1) *
        VALUE_MULTIPLIER(ch);
}

float Processing::CalculateVoltageMin(Channel::E ch)
{
    float min = CalculateMinRel(ch);
    EXIT_IF_ERROR_FLOAT(min);
    if(MEAS_MARKED == Measure::VoltageMin)
    {
        markerHor[ch][0] = static_cast<int>(min);          // Здесь не округляем, потому что min может быть только целым
    }

    return POINT_2_VOLTAGE(min, dataSet->range[ch], (ch == ChA) ? dataSet->r_shift_a : dataSet->rShiftCh1) *
        VALUE_MULTIPLIER(ch);
}

float Processing::CalculateVoltagePic(Channel::E ch)
{
    float max = CalculateVoltageMax(ch);
    float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(MEAS_MARKED == Measure::VoltagePic)
    {
        markerHor[ch][0] = static_cast<int>(CalculateMaxRel(ch));
        markerHor[ch][1] = static_cast<int>(CalculateMinRel(ch));
    }
    return max - min;
}

float Processing::CalculateVoltageMinSteady(Channel::E ch)
{
    float min = CalculateMinSteadyRel(ch);
    EXIT_IF_ERROR_FLOAT(min);
    if(MEAS_MARKED == Measure::VoltageMinSteady)
    {
        markerHor[ch][0] = static_cast<int>(ROUND(min));
    }

    return (POINT_2_VOLTAGE(min, dataSet->range[ch], (ch == ChA) ?
        dataSet->r_shift_a :
        dataSet->rShiftCh1) * VALUE_MULTIPLIER(ch));
}

float Processing::CalculateVoltageMaxSteady(Channel::E ch)
{
    float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max);

    if(MEAS_MARKED == Measure::VoltageMaxSteady)
    {
        markerHor[ch][0] = static_cast<int>(max);
    }

    Range::E range = dataSet->range[ch];
    uint rShift = (ch == ChA) ? dataSet->r_shift_a : dataSet->rShiftCh1;

    return (POINT_2_VOLTAGE(max, range, rShift) * VALUE_MULTIPLIER(ch));
}

float Processing::CalculateVoltageVybrosPlus(Channel::E ch)
{
    float max = CalculateMaxRel(ch);
    float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady);

    if (MEAS_MARKED == Measure::VoltageVybrosPlus)
    {
        markerHor[ch][0] = static_cast<int>(max);
        markerHor[ch][1] = static_cast<int>(maxSteady);
    }

    int16 rShift = (ch == ChA) ? (int16)dataSet->r_shift_a : (int16)dataSet->rShiftCh1;

    return std::fabsf(POINT_2_VOLTAGE(maxSteady, dataSet->range[ch], rShift) -
        POINT_2_VOLTAGE(max, dataSet->range[ch], rShift)) * VALUE_MULTIPLIER(ch);
}

float Processing::CalculateVoltageVybrosMinus(Channel::E ch)
{
    float min = CalculateMinRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);
    EXIT_IF_ERRORS_FLOAT(min, minSteady);

    if (MEAS_MARKED == Measure::VoltageVybrosMinus)
    {
        markerHor[ch][0] = static_cast<int>(min);
        markerHor[ch][1] = static_cast<int>(minSteady);
    }

    int16 rShift = (ch == ChA) ? (int16)dataSet->r_shift_a : (int16)dataSet->rShiftCh1;

    return std::fabsf(POINT_2_VOLTAGE(minSteady, dataSet->range[ch], rShift) -
        POINT_2_VOLTAGE(min, dataSet->range[ch], rShift)) * VALUE_MULTIPLIER(ch);
}

float Processing::CalculateVoltageAmpl(Channel::E ch)
{
    float max = CalculateVoltageMaxSteady(ch);
    float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(MEAS_MARKED == Measure::VoltageAmpl)
    {
        markerHor[ch][0] = static_cast<int>(CalculateMaxSteadyRel(ch));
        markerHor[ch][1] = static_cast<int>(CalculateMinSteadyRel(ch));
    }
    return max - min;
}

float Processing::CalculateVoltageAverage(Channel::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    int sum = 0;
    uint8 *data = &dataIn[ch][firstP];
    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = static_cast<uint8>((float)sum / period);

    if(MEAS_MARKED == Measure::VoltageAverage)
    {
        markerHor[ch][0] = aveRel;
    }

    return (POINT_2_VOLTAGE(aveRel, dataSet->range[ch], (ch == ChA) ? dataSet->r_shift_a : dataSet->rShiftCh1) *
        VALUE_MULTIPLIER(ch));
}

float Processing::CalculateVoltageRMS(Channel::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    float rms = 0.0F;

    int16 rShift = (ch == ChA) ? (int16)dataSet->r_shift_a : (int16)dataSet->rShiftCh1;

    for(uint i = firstP; i < firstP + period; i++)
    {
        float volts = POINT_2_VOLTAGE(dataIn[ch][i], dataSet->range[ch], rShift);
        rms +=  volts * volts;
    }

    if(MEAS_MARKED == Measure::VoltageRMS)
    {
        markerHor[ch][0] = MathFPGA::VoltageToPoint(std::sqrtf(rms / period), dataSet->range[ch], rShift);
    }

    return std::sqrtf(rms / period) * VALUE_MULTIPLIER(ch);
}

float Processing::CalculatePeriod(Channel::E ch)
{
    static float period[2] = {0.0F, 0.0F};

    if(!periodIsCaclulating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == ERROR_VALUE_UINT8)
        {
            period[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));
            float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown);

            float firstIntersection = intersectionDownToTop < intersectionTopToDown ? intersectionDownToTop :
                intersectionTopToDown;

            float secondIntersection = FindIntersectionWithHorLine(ch, 2, intersectionDownToTop < intersectionTopToDown,
                static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

            float per = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);

            period[ch] = per;
            periodIsCaclulating[ch] = true;
        }
    }

    return period[ch];
}

#define EXIT_FROM_PERIOD_ACCURACY               \
    period[ch] = ERROR_VALUE_INT;             \
    periodAccurateIsCalculating[ch] = true;   \
    return period[ch];

int Processing::CalculatePeriodAccurately(Channel::E ch)
{
    static int period[2];

    int sums[FPGA_MAX_POINTS];

    if(!periodAccurateIsCalculating[ch])
    {
        period[ch] = 0;
        float pic = CalculatePicRel(ch);

        if(pic == ERROR_VALUE_FLOAT)
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = static_cast<int>(pic * 5);
        sums[firstP] = dataIn[ch][firstP];

        uint i = firstP + 1;
        int *sum = &sums[i];
        uint8 *data = &dataIn[ch][i];
        uint8 *end = &dataIn[ch][lastP];

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

        int addShift = (int)firstP - 1;
        int maxPeriod = static_cast<int>(numP * 0.95F);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int s = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = (int)numP - nextPeriod;

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
                period[ch] = nextPeriod;
            }
        }

        if(period[ch] == 0)
        {
            period[ch] = ERROR_VALUE_INT;
        }
        periodAccurateIsCalculating[ch] = true;
    }

    return period[ch];
}

float Processing::CalculateFreq(Channel::E ch)
{
    float period = CalculatePeriod(ch);
    return period == ERROR_VALUE_FLOAT ? ERROR_VALUE_FLOAT : 1.0F / period;
}

float Processing::FindIntersectionWithHorLine(Channel::E ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = (int)firstP;
    int compValue = (int)lastP - 1;

    uint8 *data = &dataIn[ch][0];

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

float Processing::CalculateDurationPlus(Channel::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue);

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, static_cast<uint8>(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);
}

float Processing::CalculateDurationMinus(Channel::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue);

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, static_cast<uint8>(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);
}

float Processing::CalculateTimeNarastaniya(Channel::E ch)   // WARN Здесь, возможно, нужно увеличить точность - брать не
                                // целые значени расстояний между отсчётами по времени, а рассчитывать пересечения линий
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(min01));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, static_cast<uint8>(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);

    if (MEAS_MARKED == Measure::TimeNarastaniya)
    {
        markerHor[ch][0] = static_cast<int>(max09);
        markerHor[ch][1] = static_cast<int>(min01);
        markerVert[ch][0] = static_cast<int>(firstIntersection - SHIFT_IN_MEMORY);
        markerVert[ch][1] = static_cast<int>(secondIntersection - SHIFT_IN_MEMORY);
    }

    return retValue;
}

float Processing::CalculateTimeSpada(Channel::E ch)                          // WARN Аналогично времени нарастания
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(max09));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, static_cast<uint8>(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TSHIFT_2_ABS((secondIntersection - firstIntersection) / 2.0F, dataSet->tBase);

    if (MEAS_MARKED == Measure::TimeSpada)
    {
        markerHor[ch][0] = static_cast<int>(max09);
        markerHor[ch][1] = static_cast<int>(min01);
        markerVert[ch][0] = static_cast<int>(firstIntersection - SHIFT_IN_MEMORY);
        markerVert[ch][1] = static_cast<int>(secondIntersection - SHIFT_IN_MEMORY);
    }

    return retValue;
}

float Processing::CalculateSkvaznostPlus(Channel::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationPlus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}

float Processing::CalculateSkvaznostMinus(Channel::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}

float Processing::CalculateMinSteadyRel(Channel::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    if(!minSteadyIsCalculating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == ERROR_VALUE_FLOAT)
        {
            min[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[ch][firstP];
            puchar const end = &dataIn[ch][lastP];
            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            min[ch] = (float)sum / numSums;
            int numMin = numSums; //-V2551

            float pic = CalculatePicRel(ch);
            if (pic == ERROR_VALUE_FLOAT)
            {
                min[ch] = ERROR_VALUE_FLOAT;
            }
            else
            {
                int numDeleted = 0;
                float value = pic / 9.0F;
                data = &dataIn[ch][firstP];
                float _min = min[ch];
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
                min[ch] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(ch) : (float)sum / numSums;
            }
        }
        minSteadyIsCalculating[ch] = true;
    }

    return min[ch];
}

float Processing::CalculateMaxSteadyRel(Channel::E ch)
{
    static float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        
        if(aveValue == ERROR_VALUE_FLOAT)
        {
            max[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[ch][firstP];
            puchar const end = &dataIn[ch][lastP];
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            max[ch] = (float)sum / numSums;

            float pic = CalculatePicRel(ch);

            if (pic == ERROR_VALUE_FLOAT)
            {
                max[ch] = ERROR_VALUE_FLOAT;
            }
            else
            {
                int numDeleted = 0;
                int numMax = numSums;
                float value = pic / 9.0F;

                data = &dataIn[ch][firstP];
                uint8 _max = static_cast<uint8>(max[ch]);
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
                max[ch] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : (float)sum / numSums;
            }
        }
        maxSteadyIsCalculating[ch] = true;
    }

    return max[ch];
}

float Processing::CalculateMaxRel(Channel::E ch)
{
    static float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[ch])
    {
        uint8 val = MathFPGA::GetMaxFromArrayWithErrorCode(dataIn[ch], firstP, lastP);
        max[ch] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        maxIsCalculating[ch] = true;
    }

    return max[ch];
}

float Processing::CalculateMinRel(Channel::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[ch])
    {
        uint8 val = MathFPGA::GetMinFromArrayWithErrorCode(dataIn[ch], firstP, lastP);
        min[ch] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        minIsCalculating[ch] = true;
    }

    return min[ch];
}

float Processing::CalculateAverageRel(Channel::E ch)
{
    static float ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[ch])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        ave[ch] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : (min + max) / 2.0F;
        aveIsCalculating[ch] = true;
    }
    return ave[ch];
}

float Processing::CalculatePicRel(Channel::E ch)
{
    static float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[ch])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        pic[ch] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : max - min;
        picIsCalculating[ch] = true;
    }
    return pic[ch];
}

float Processing::CalculateDelayPlus(Channel::E ch)
{
    float period0 = CalculatePeriod(ChA.value);
    float period1 = CalculatePeriod(ChB.value);

    EXIT_IF_ERRORS_FLOAT(period0, period1);
    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return ERROR_VALUE_FLOAT;
    }

    float average0 = CalculateAverageRel(ChA.value);
    float average1 = CalculateAverageRel(ChB.value);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = (ch == ChA) ? average0 : average1;
    float averageSecond = (ch == ChA) ? average1 : average0;
    Channel::E firstChannel = (ch == ChA) ? ChA.value : ChB.value;
    Channel::E secondChannel = (ch == ChA) ? ChB.value : ChA.value;

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

float Processing::CalculateDelayMinus(Channel::E ch)
{
    float period0 = CalculatePeriod(ChA.value);
    float period1 = CalculatePeriod(ChB.value);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return ERROR_VALUE_FLOAT;
    }

    float average0 = CalculateAverageRel(ChA.value);
    float average1 = CalculateAverageRel(ChB.value);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = (ch == ChA) ? average0 : average1;
    float averageSecond = (ch == ChA) ? average1 : average0;
    Channel::E firstChannel = (ch == ChA) ? ChA.value : ChB.value;
    Channel::E secondChannel = (ch == ChA) ? ChB.value : ChA.value;

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

float Processing::CalculatePhazaPlus(Channel::E ch)
{
    float delay = CalculateDelayPlus(ch);
    float period = CalculatePeriod(ch);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT)
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0F;
}

float Processing::CalculatePhazaMinus(Channel::E ch)
{
    float delay = CalculateDelayMinus(ch);
    float period = CalculatePeriod(ch);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT)
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0F; 
}

void Processing::SetSignal(puchar data0, puchar data1, DataSettings *ds, int _firstPoint, int _lastPoint)
{
    firstP = (uint)_firstPoint;
    lastP = (uint)_lastPoint;
    numP = lastP - firstP;
    dataSet = ds;

    if (ds == nullptr)
    {
        return;
    }
    
    uint numSmoothing = Smoothing::NumPoints();

    uint length = ds->BytesInChannel() * (ds->peakDet == PeackDetMode::Disable ? 1 : 2);

    Math::CalculateFiltrArray(data0, &dataIn[ChA][0], (int)length, (int)numSmoothing);
    Math::CalculateFiltrArray(data1, &dataIn[ChB][0], (int)length, (int)numSmoothing);

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

float Processing::GetCursU(Channel::E ch, float posCurT)
{   
    int first = 0;
    int last = 0;
    SettingsDisplay::PointsOnDisplay(&first, &last);

    float retValue;
    LIMITATION(retValue, static_cast<float>(200.0F - (dataIn[ch])[first + (int)posCurT] + MIN_VALUE), 0.0F, 200.0F);
    return retValue;
}

float Processing::GetCursT(Channel::E ch, float posCurU, int numCur)
{
    int firstPoint = 0;
    int lastPoint = 0;
    SettingsDisplay::PointsOnDisplay(&firstPoint, &lastPoint);

    int prevData = 200 - (dataIn[ch])[firstPoint] + MIN_VALUE;

    int numIntersections = 0;

    for(int i = firstPoint + 1; i < lastPoint; i++)
    {
        int curData = 200 - (dataIn[ch])[i] + MIN_VALUE;

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

void Processing::InterpolationSinX_X(uint16 data[FPGA_MAX_POINTS], TBase::E tBase)
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
            signedData[numSignedPoints] = (uint8)data[pos];
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
            float sinX = (part < delta - 1) ?
                std::sin(PI / delta * (part + 1)) :
                std::sin(PI / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)    // Здесь используем более быструю, но более неправильную арифметику целвых чисел
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
            else                     // На этих развёртках арифметика с плавающей запятой даёт приемлемое быстродействие
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

String Processing::GetStringMeasure(Measure::E measure, Channel::E ch)
{
    if (!SET_ENABLED(ch))
    {
        return String("");
    }

    String result("%s", (ch == ChA) ? "1: " : "2: ");

    if(dataSet == 0)
    {
        result.Append("-.-");
    }
    else if((ch == ChA && !dataSet->IsEnabled(ChA)) || (ch == ChB && !dataSet->IsEnabled(ChB)))
    {
    }
    else if(measures[measure].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncCFBC func = measures[measure].FucnConvertate;
        float value = values[measure].value[ch];
        char *text = func(value, measures[measure].showSign, bufferForFunc);
        result.Append(text);
    }

    return result;
}

int Processing::GetMarkerHorizontal(Channel::E ch, int numMarker)
{
    return markerHor[ch][numMarker] - MIN_VALUE;
}

int Processing::GetMarkerVertical(Channel::E ch, int numMarker)
{
    return markerVert[ch][numMarker];
}

void Processing::CountedToCurrentSettings()
{
    std::memset(dataOut0, 0, FPGA_MAX_POINTS);
    std::memset(dataOut1, 0, FPGA_MAX_POINTS);
    
    int numPoints = (int)dataSet->BytesInChannel() * (dataSet->peakDet == PeackDetMode::Disable ? 1 : 2);

    int16 dataTShift = dataSet->tShift;
    int16 curTShift = TShift::Get();

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
 
    if (dataSet->IsEnabled(ChA) && (dataSet->range[0] != SET_RANGE_A || dataSet->r_shift_a != (uint)RShift::Get(ChA)))
    {
        Range::E range = SET_RANGE_A;
        int16 rShift = RShift::Get(ChA);

        for (int i = 0; i < numPoints; i++)
        {
            float absValue = POINT_2_VOLTAGE(dataOut0[i], dataSet->range[0], dataSet->r_shift_a);
            int relValue = static_cast<int>((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) /
                MathFPGA::voltsInPixel[range] + MIN_VALUE);

            if (relValue < MIN_VALUE)       { dataOut0[i] = MIN_VALUE; }
            else if (relValue > MAX_VALUE)  { dataOut0[i] = MAX_VALUE; }
            else                            { dataOut0[i] = (uint8)relValue; }
        }
    }
    if (dataSet->IsEnabled(ChB) && (dataSet->range[1] != SET_RANGE_B || dataSet->rShiftCh1 != (uint)RShift::Get(ChB)))
    {
        Range::E range = SET_RANGE_B;
        int16 rShift = RShift::Get(ChB);

        for (int i = 0; i < numPoints; i++)
        {
            float absValue = POINT_2_VOLTAGE(dataOut1[i], dataSet->range[1], dataSet->rShiftCh1);
            int relValue = static_cast<int>((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RSHIFT_2_ABS(rShift, range)) /
                MathFPGA::voltsInPixel[range] + MIN_VALUE);

            if (relValue < MIN_VALUE)       { dataOut1[i] = MIN_VALUE; }
            else if (relValue > MAX_VALUE)  { dataOut1[i] = MAX_VALUE; }
            else                            { dataOut1[i] = (uint8)relValue; }
        }
    }
}
