// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Storage.h"
#include "Utils/ProcessingSignal.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>


struct MeasureValue
{
    float value[2];
};


static Buffer<uint8> in[Channel::Count];        // ���� �������� ������� ������ ����� ������������� �������
static Buffer<uint8> out[Channel::Count];       // ���� �������� ������ ����� ���������� � ������� ����������

static DataSettings *pds = nullptr;

static uint firstP = 0;
static uint lastP = 0;
static uint numP = 0;


static char *FloatFract2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, Float(value).ToText(always_sign, 4).c_str());
}

static char *Voltage2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, Voltage(value).ToText(always_sign).c_str());
}

static char *Time2StringC(float value, bool always_sign, char *buffer)
{
    return std::strcpy(buffer, Time(value).ToText(always_sign).c_str());
}

static char *Freq2StringC(float value, bool, char *buffer)
{
    return std::strcpy(buffer, Frequency(value).ToText().c_str());
}

static char *Phase2StringC(float value, bool, char *buffer)
{
    return std::strcpy(buffer, Phase(value).ToText().c_str());
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

static Int markerHor[Channel::Count][2]  = {{Int(false)}, {Int(false)}};
static Int markerVert[Channel::Count][2] = {{Int(false)}, {Int(false)}};

static bool maxIsCalculating[2] = {false, false};
static bool minIsCalculating[2] = {false, false};
static bool maxSteadyIsCalculating[2] = {false, false};
static bool minSteadyIsCalculating[2] = {false, false};
static bool aveIsCalculating[2] = {false, false};
static bool periodIsCaclulating[2] = {false, false};
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = {false, false};

#define EXIT_IF_ERROR_FLOAT(x)      if(!x.IsValid()) { return x; }

#define EXIT_IF_ERRORS_FLOAT(x, y)  if(!x.IsValid() || !y.IsValid())  { return x; }

#define EXIT_IF_ERROR_INT(x)        if(!x.IsValid()) { return InvalidFloat(); }

void Processing::CalculateMeasures()
{
    if(!set.measures.show || !pds)
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
            pFuncCalculate func = measures[meas].FuncCalculate;
            if(func)
            {
                if(set.measures.marked.Is(meas) || set.measures.marked.IsNone())
                {
                    markerVert[ChA][0].SetInvalid();
                    markerVert[ChA][1].SetInvalid();
                    markerVert[ChB][0].SetInvalid();
                    markerVert[ChB][1].SetInvalid();
                    markerHor[ChA][0].SetInvalid();
                    markerHor[ChA][1].SetInvalid();
                    markerHor[ChB][0].SetInvalid();
                    markerHor[ChB][1].SetInvalid();
                }

                if(set.measures.source.IsA() || set.measures.source.IsAB())
                {
                    values[meas].value[ChA] = func(ChA.value);
                }

                if(set.measures.source.IsB() || set.measures.source.IsAB())
                {
                    values[meas].value[ChB] = func(ChB.value);
                }
            }
        }
    }
}


void Processing::LeadToCurrentSetings(DataReading &dr)
{
    UNUSED(dr);

#ifndef GUI

    BitSet64 p = SettingsDisplay::BytesOnDisplay();

    firstP = (uint)p.first;
    lastP = (uint)p.second;
    numP = lastP - firstP;
    pds = &dr.Settings();

    int numSmoothing = (int)Smoothing::NumPoints();

    Buffer<uint8> outA(numP);
    Buffer<uint8> outB(numP);

    Math::CalculateFiltrArray(dr.Data(ChA) + firstP, outA.Data(), (int)numP, numSmoothing);
    Math::CalculateFiltrArray(dr.Data(ChB) + firstP, outB.Data(), (int)numP, numSmoothing);

    std::memcpy(dr.Data(ChA) + firstP, outA.Data(), numP);
    std::memcpy(dr.Data(ChB) + firstP, outB.Data(), numP);

//    CountedToCurrentSettings();

#endif
}


void Processing::CountedToCurrentSettings(DataReading &dr)
{
    DataSettings &ds = dr.Settings();

    out[ChA].Realloc(ds.BytesInChannel());
    out[ChB].Realloc(ds.BytesInChannel());

    int numPoints = (int)in[ChA].Size();

    int dataTShift = pds->tshift;

    int dTShift = set.time.shift - dataTShift;

    for (int i = 0; i < numPoints; i++)
    {
        int index = i - dTShift;
        if (index >= 0 && index < numPoints)
        {
            out[ChA][index] = in[0][i];
            out[ChB][index] = in[1][i];
        }
    }

    if (pds->IsEnabled(ChA) &&
        (pds->range[0] != set.chan[Channel::A].range || pds->r_shift_a != (uint)set.chan[Channel::A].rshift))
    {
        Range::E range = set.chan[Channel::A].range;
        int16 rShift = set.chan[Channel::A].rshift;

        for (int i = 0; i < numPoints; i++)
        {
            float absValue = Value::ToVoltage(out[ChA][i], pds->range[0], (int16)pds->r_shift_a);
            int relValue = (int)((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RShift::ToAbs(rShift, range)) /
                MathFPGA::voltsInPixel[range] + Value::MIN);

            if (relValue < Value::MIN) { out[ChA][i] = Value::MIN; }
            else if (relValue > Value::MAX) { out[ChA][i] = Value::MAX; }
            else { out[ChA][i] = (uint8)relValue; }
        }
    }
    if (pds->IsEnabled(ChB) &&
        (pds->range[1] != set.chan[Channel::B].range || pds->r_shift_b != (uint)set.chan[Channel::B].rshift))
    {
        Range::E range = set.chan[Channel::B].range;
        int16 rShift = set.chan[Channel::B].rshift;

        for (int i = 0; i < numPoints; i++)
        {
            float absValue = Value::ToVoltage(out[ChB][i], pds->range[1], (int16)pds->r_shift_b);
            int relValue = (int)((absValue + MAX_VOLTAGE_ON_SCREEN(range) + RShift::ToAbs(rShift, range)) /
                MathFPGA::voltsInPixel[range] + Value::MIN);

            if (relValue < Value::MIN) { out[ChB][i] = Value::MIN; }
            else if (relValue > Value::MAX) { out[ChB][i] = Value::MAX; }
            else { out[ChB][i] = (uint8)relValue; }
        }
    }
}


Float Processing::CalculateVoltageMax(Channel::E ch)
{
    Float max = CalculateMaxRel(ch);
    
    EXIT_IF_ERROR_FLOAT(max);

    if(set.measures.marked.Is(Measure::VoltageMax))
    {
        markerHor[ch][0] = (int)(max);         // ����� �� ���������, ������ ��� max ����� ���� ������ �����
    }

    return Value::ToVoltage((uint8)max, pds->range[ch], (int16)((ch == ChA) ? pds->r_shift_a : pds->r_shift_b)) *
        Divider::ToAbs(ch);
}


Float Processing::CalculateVoltageMin(Channel::E ch)
{
    Float min = CalculateMinRel(ch);

//    if (ch == Channel::A)
//    {
//        LOG_WRITE("min = %f", min.value);
//    }

    EXIT_IF_ERROR_FLOAT(min);

    if(set.measures.marked.Is(Measure::VoltageMin))
    {
        markerHor[ch][0] = (int)(min);          // ����� �� ���������, ������ ��� min ����� ���� ������ �����
    }

    Float result = Value::ToVoltage((uint8)min, pds->range[ch], (int16)((ch == ChA) ? pds->r_shift_a : pds->r_shift_b)) *
        Divider::ToAbs(ch);

//    if (ch == Channel::A)
//    {
//        LOG_WRITE("min = %d, result = %f", (uint8)min, result.value);
//    }

    return result;
}

Float Processing::CalculateVoltagePic(Channel::E ch)
{
    Float max = CalculateVoltageMax(ch);
    Float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(set.measures.marked.Is(Measure::VoltagePic))
    {
        markerHor[ch][0] = (int)(CalculateMaxRel(ch));
        markerHor[ch][1] = (int)(CalculateMinRel(ch));
    }
    return max - min;
}

Float Processing::CalculateVoltageMinSteady(Channel::E ch)
{
    Float min = CalculateMinSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(min);

    if(set.measures.marked.Is(Measure::VoltageMinSteady))
    {
        markerHor[ch][0] = (int)min.Round();
    }

    return (Value::ToVoltage((uint8)min, pds->range[ch], (ch == ChA) ?
        (int16)pds->r_shift_a :
        (int16)pds->r_shift_b) * Divider::ToAbs(ch));
}

Float Processing::CalculateVoltageMaxSteady(Channel::E ch)
{
    Float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max);

    if(set.measures.marked.Is(Measure::VoltageMaxSteady))
    {
        markerHor[ch][0] = (int)(max);
    }

    Range::E range = pds->range[ch];
    uint rShift = (ch == ChA) ? pds->r_shift_a : pds->r_shift_b;

    return (Value::ToVoltage((uint8)max, range, (int16)rShift) * Divider::ToAbs(ch));
}

Float Processing::CalculateVoltageVybrosPlus(Channel::E ch)
{
    Float max = CalculateMaxRel(ch);
    Float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady);

    if (set.measures.marked.Is(Measure::VoltageVybrosPlus))
    {
        markerHor[ch][0] = (int)(max);
        markerHor[ch][1] = (int)(maxSteady);
    }

    int16 rShift = (ch == ChA) ? (int16)pds->r_shift_a : (int16)pds->r_shift_b;

    return std::fabsf(Value::ToVoltage((uint8)maxSteady, pds->range[ch], (int16)rShift) -
        Value::ToVoltage((uint8)max, pds->range[ch], (int16)rShift)) * Divider::ToAbs(ch);
}

Float Processing::CalculateVoltageVybrosMinus(Channel::E ch)
{
    Float min = CalculateMinRel(ch);
    Float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(min, minSteady);

    if (set.measures.marked.Is(Measure::VoltageVybrosMinus))
    {
        markerHor[ch][0] = (int)(min);
        markerHor[ch][1] = (int)(minSteady);
    }

    int16 rShift = (ch == ChA) ? (int16)pds->r_shift_a : (int16)pds->r_shift_b;

    return std::fabsf(Value::ToVoltage((uint8)minSteady, pds->range[ch], (int16)rShift) -
        Value::ToVoltage((uint8)min, pds->range[ch], (int16)rShift)) * Divider::ToAbs(ch);
}

Float Processing::CalculateVoltageAmpl(Channel::E ch)
{
    Float max = CalculateVoltageMaxSteady(ch);
    Float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max);

    if(set.measures.marked.Is(Measure::VoltageAmpl))
    {
        markerHor[ch][0] = (int)(CalculateMaxSteadyRel(ch));
        markerHor[ch][1] = (int)(CalculateMinSteadyRel(ch));
    }
    return max - min;
}

Float Processing::CalculateVoltageAverage(Channel::E ch)
{
    Int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    int sum = 0;
    uint8 *data = &in[ch][firstP];

    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = (uint8)((float)sum / period);

    if(set.measures.marked.Is(Measure::VoltageAverage))
    {
        markerHor[ch][0] = aveRel;
    }

    return (Value::ToVoltage((uint8)aveRel, pds->range[ch], (ch == ChA) ?
        (int16)pds->r_shift_a :
        (int16)pds->r_shift_b) *
        Divider::ToAbs(ch));
}

Float Processing::CalculateVoltageRMS(Channel::E ch)
{
    Int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period);

    float rms = 0.0F;

    int16 rShift = (ch == ChA) ? (int16)pds->r_shift_a : (int16)pds->r_shift_b;

    for(uint i = firstP; i < firstP + period; i++)
    {
        float volts = Value::ToVoltage(in[ch][i], pds->range[ch], rShift);
        rms +=  volts * volts;
    }

    if(set.measures.marked.Is(Measure::VoltageRMS))
    {
        markerHor[ch][0] = Value::FromVoltage(std::sqrtf(rms / period), pds->range[ch], rShift);
    }

    return std::sqrtf(rms / period) * Divider::ToAbs(ch);
}

Float Processing::CalculatePeriod(Channel::E ch)
{
    static Float period[2] = {0.0F, 0.0F};

    if(!periodIsCaclulating[ch])
    {
        Float aveValue = CalculateAverageRel(ch);

        if(!aveValue.IsValid())
        {
            period[ch].SetInvalid();
        }
        else
        {
            Float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);
            Float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown);

            Float firstIntersection = intersectionDownToTop < intersectionTopToDown ? intersectionDownToTop :
                intersectionTopToDown;

            Float secondIntersection = FindIntersectionWithHorLine(ch, 2, intersectionDownToTop < intersectionTopToDown,
                (uint8)(aveValue));

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

            float per = TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());

            period[ch] = per;
            periodIsCaclulating[ch] = true;
        }
    }

    return period[ch];
}

#define EXIT_FROM_PERIOD_ACCURACY               \
    period[ch].SetInvalid();                    \
    periodAccurateIsCalculating[ch] = true;     \
    return period[ch];

Int Processing::CalculatePeriodAccurately(Channel::E ch)
{
    static Int period[2];

    Buffer<int> sums(out[ch].Size());

    if(!periodAccurateIsCalculating[ch])
    {
        period[ch] = 0;
        Float pic = CalculatePicRel(ch);

        if(!pic.IsValid())
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = (int)(pic * 5);
        sums[firstP] = in[ch][firstP];

        uint i = firstP + 1;
        int *sum = &sums[i];
        uint8 *data = &in[ch][i];
        uint8 *end = &in[ch][lastP];

        while (data < end)
        {
            uint8 point = *data++;
            if(point < Value::MIN || point >= Value::MAX)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *sum = *(sum - 1) + point;
            sum++;
        }

        int addShift = (int)firstP - 1;
        int maxPeriod = (int)(numP * 0.95F);

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
            period[ch].SetInvalid();
        }
        periodAccurateIsCalculating[ch] = true;
    }

    return period[ch];
}

Float Processing::CalculateFreq(Channel::E ch)
{
    Float period = CalculatePeriod(ch);

    if (period.IsValid())
    {
        period = 1.0f / period;
    }

    return period;
}

Float Processing::FindIntersectionWithHorLine(Channel::E ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = (int)firstP;
    int compValue = (int)lastP - 1;

    uint8 *data = &in[ch][0];

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
        return InvalidFloat();
    }
    return Math::GetIntersectionWithHorizontalLine(x, data[x], x + 1, data[x + 1], yLine);
}

Float Processing::CalculateDurationPlus(Channel::E ch)
{
    Float aveValue = CalculateAverageRel(ch);

    EXIT_IF_ERROR_FLOAT(aveValue);

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}

Float Processing::CalculateDurationMinus(Channel::E ch)
{
    Float aveValue = CalculateAverageRel(ch);

    EXIT_IF_ERROR_FLOAT(aveValue);

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(aveValue));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)(aveValue));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}

Float Processing::CalculateTimeNarastaniya(Channel::E ch)   // WARN �����, ��������, ����� ��������� �������� - ����� ��
                                // ����� ������� ���������� ����� ��������� �� �������, � ������������ ����������� �����
{
    Float maxSteady = CalculateMaxSteadyRel(ch);
    Float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(min01));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float retValue = TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());

    if (set.measures.marked.Is(Measure::TimeNarastaniya))
    {
        markerHor[ch][0] = (int)(max09);
        markerHor[ch][1] = (int)(min01);
        markerVert[ch][0] = (int)(firstIntersection - set.display.shift_in_memory_in_points);
        markerVert[ch][1] = (int)(secondIntersection - set.display.shift_in_memory_in_points);
    }

    return retValue;
}

Float Processing::CalculateTimeSpada(Channel::E ch)                          // WARN ���������� ������� ����������
{
    Float maxSteady = CalculateMaxSteadyRel(ch);
    Float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady);

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    Float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(max09));
    Float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    float result = TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());

    if (set.measures.marked.Is(Measure::TimeSpada))
    {
        markerHor[ch][0] = (int)(max09);
        markerHor[ch][1] = (int)(min01);
        markerVert[ch][0] = (int)(firstIntersection - set.display.shift_in_memory_in_points);
        markerVert[ch][1] = (int)(secondIntersection - set.display.shift_in_memory_in_points);
    }

    return result;
}

Float Processing::CalculateSkvaznostPlus(Channel::E ch)
{
    Float period = CalculatePeriod(ch);
    Float duration = CalculateDurationPlus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}

Float Processing::CalculateSkvaznostMinus(Channel::E ch)
{
    Float period = CalculatePeriod(ch);
    Float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration);

    return period / duration;
}

float Processing::CalculateMinSteadyRel(Channel::E ch)
{
    static Float min[2] = {255.0F, 255.0F};

    if(!minSteadyIsCalculating[ch])
    {
        Float aveValue = CalculateAverageRel(ch);

        if(!aveValue.IsValid())
        {
            min[ch].SetInvalid();
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &in[ch][firstP];
            puchar const end = &in[ch][lastP];

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
            int numMin = numSums;

            Float pic = CalculatePicRel(ch);

            if (!pic.IsValid())
            {
                min[ch].SetInvalid();
            }
            else
            {
                int numDeleted = 0;
                float value = pic / 9.0F;
                data = &in[ch][firstP];
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

                min[ch] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(ch) : Float((float)sum / numSums);
            }
        }
        minSteadyIsCalculating[ch] = true;
    }

    return min[ch];
}

float Processing::CalculateMaxSteadyRel(Channel::E ch)
{
    static Float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[ch])
    {
        Float aveValue = CalculateAverageRel(ch);
        
        if(!aveValue.IsValid())
        {
            max[ch].SetInvalid();
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &in[ch][firstP];
            puchar const end = &in[ch][lastP];
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

            Float pic = CalculatePicRel(ch);

            if (!pic.IsValid())
            {
                max[ch].SetInvalid();
            }
            else
            {
                int numDeleted = 0;
                int numMax = numSums;
                float value = pic / 9.0F;

                data = &in[ch][firstP];
                uint8 _max = (uint8)(max[ch]);
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

                max[ch] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : Float((float)sum / numSums);
            }
        }
        maxSteadyIsCalculating[ch] = true;
    }

    return max[ch];
}

Float Processing::CalculateMaxRel(Channel::E ch)
{
    static Float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[ch])
    {
        max[ch] = MathFPGA::GetMaxFromArrayWithErrorCode(in[ch].DataU8(), firstP, lastP);

        maxIsCalculating[ch] = true;
    }

    return max[ch];
}

Float Processing::CalculateMinRel(Channel::E ch)
{
    static Float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[ch])
    {
        min[ch] = MathFPGA::GetMinFromArrayWithErrorCode(in[ch].DataU8(), firstP, lastP);

        minIsCalculating[ch] = true;
    }

    return min[ch];
}

Float Processing::CalculateAverageRel(Channel::E ch)
{
    static Float ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[ch])
    {
        Float min = CalculateMinRel(ch);
        Float max = CalculateMaxRel(ch);

        if (!min.IsValid() || !max.IsValid())
        {
            ave[ch].SetInvalid();
        }
        else
        {
            ave[ch] = (min + max) / 2.0F;
        }

        aveIsCalculating[ch] = true;
    }

    return ave[ch];
}

Float Processing::CalculatePicRel(Channel::E ch)
{
    static Float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[ch])
    {
        Float min = CalculateMinRel(ch);
        Float max = CalculateMaxRel(ch);

        if (!min.IsValid() || !max.IsValid())
        {
            pic[ch].SetInvalid();
        }
        else
        {
            pic[ch] = max - min;
        }

        picIsCalculating[ch] = true;
    }
    return pic[ch];
}

Float Processing::CalculateDelayPlus(Channel::E ch)
{
    Float period0 = CalculatePeriod(ChA.value);
    Float period1 = CalculatePeriod(ChB.value);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return InvalidFloat();
    }

    Float average0 = CalculateAverageRel(ChA.value);
    Float average1 = CalculateAverageRel(ChB.value);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = (ch == ChA) ? average0 : average1;
    float averageSecond = (ch == ChA) ? average1 : average0;

    Channel::E firstChannel = (ch == ChA) ? ChA.value : ChB.value;
    Channel::E secondChannel = (ch == ChA) ? ChB.value : ChA.value;

    Float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, (uint8)averageFirst);
    Float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, (uint8)(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}

Float Processing::CalculateDelayMinus(Channel::E ch)
{
    Float period0 = CalculatePeriod(ChA.value);
    Float period1 = CalculatePeriod(ChB.value);

    EXIT_IF_ERRORS_FLOAT(period0, period1);

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return InvalidFloat();
    }

    Float average0 = CalculateAverageRel(ChA.value);
    Float average1 = CalculateAverageRel(ChB.value);

    EXIT_IF_ERRORS_FLOAT(average0, average1);

    float averageFirst = (ch == ChA) ? average0 : average1;
    float averageSecond = (ch == ChA) ? average1 : average0;
    Channel::E firstChannel = (ch == ChA) ? ChA.value : ChB.value;
    Channel::E secondChannel = (ch == ChA) ? ChB.value : ChA.value;

    Float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, (uint8)averageFirst);
    Float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection);

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, (uint8)averageSecond);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection);

    return TShift::ToAbs((secondIntersection - firstIntersection) / 2.0F, pds->GetTBase());
}

Float Processing::CalculatePhazaPlus(Channel::E ch)
{
    Float delay = CalculateDelayPlus(ch);
    Float period = CalculatePeriod(ch);

    if(!delay.IsValid() || !period.IsValid())
    {
        return InvalidFloat();
    }

    return delay / period * 360.0F;
}

Float Processing::CalculatePhazaMinus(Channel::E ch)
{
    Float delay = CalculateDelayMinus(ch);
    Float period = CalculatePeriod(ch);

    if(!delay.IsValid() || !period.IsValid())
    {
        return InvalidFloat();
    }

    return delay / period * 360.0F; 
}


float Processing::GetCursU(const Channel &ch, float posCurT)
{   
    BitSet64 p = SettingsDisplay::PointsOnDisplay();

    return Math::Limitation((float)(200.0F - (in[ch])[p.first + (int)posCurT] + Value::MIN), 0.0F, 200.0F);
}


float Processing::GetCursT(const Channel &ch, float posCurU, int numCur)
{
    BitSet64 p = SettingsDisplay::PointsOnDisplay();

    int prevData = 200 - (in[ch])[p.first] + Value::MIN;

    int numIntersections = 0;

    for(int i = p.first + 1; i < p.second; i++)
    {
        int curData = 200 - (in[ch])[i] + Value::MIN;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - p.second);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - p.first);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - p.first);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - p.first);
                }
            }
        }
        prevData = curData;
    }
    return 0;
}

void Processing::InterpolationSinX_X(Buffer<uint16> &buffer, TBase::E tBase)
{
/*
     ������������������ x � sin(x)
2    1. 50�� : pi/2, -pi/2 ...
8    2. 20�� : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 10�� : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 5��  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 2��  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    int deltas[5] = {50, 20, 10, 5, 2};
    int delta = deltas[tBase];

    Buffer<uint8> signedData(buffer.Size() / 2);

    int numSignedPoints = 0;
    
    for (uint pos = 0; pos < buffer.Size(); pos++)
    {
        if (buffer[pos] > 0)
        {
            signedData[numSignedPoints] = (uint8)buffer[pos];
            numSignedPoints++;
        }
    }

    // ����� �������� ������ �������� �����

    int shift = 0;

    for (uint pos = 0; pos < buffer.Size(); pos++)
    {
        if (buffer[pos] > 0)
        {
            shift = (int)pos;
            break;
        }
    }

    float deltaX = Math::Pi;
    float stepX0 = Math::Pi / (float)delta;
    float x0 = Math::Pi - stepX0;
    int num = 0;
    
    for(uint i = 0; i < buffer.Size(); i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            buffer[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ?
                std::sin(Math::Pi / delta * (part + 1)) :
                std::sin(Math::Pi / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)    // ����� ���������� ����� �������, �� ����� ������������ ���������� ������ �����
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

                buffer[i] = (uint8)(value * KOEFF);
            }
            else                     // �� ���� ��������� ���������� � ��������� ������� ��� ���������� ��������������
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }

                buffer[i] = (uint8)value;
            }
        }
    }

    int pos = (int)(buffer.Size() - 1);

    while (pos > shift)
    {
        buffer[pos] = buffer[pos - shift];
        pos--;
    }
}

Text Processing::GetStringMeasure(Measure::E measure, const Channel &ch)
{
    if (!ch.IsEnabled())
    {
        return Text("");
    }

    Text result("%s", (ch == ChA) ? "1: " : "2: ");

    if(pds == nullptr)
    {
        result.Append("-.-");
    }
    else if((ch == ChA && !pds->IsEnabled(ChA)) || (ch == ChB && !pds->IsEnabled(ChB)))
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
    return markerHor[ch][numMarker] - Value::MIN;
}


int Processing::GetMarkerVertical(Channel::E ch, int numMarker)
{
    return markerVert[ch][numMarker];
}
