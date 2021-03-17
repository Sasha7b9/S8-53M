// 2021/03/16 16:35:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/Storage.h"
#include "Settings/Settings.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


#define WRITE_AND_OR_INVERSE(addr, data, ch)
//    if(SET_INVERSE(ch))                                                                   \
//    {                                                                                               \
//        data = (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE));    \
//    }                                                                                               \
//    *addr = (uint16)data;

/*
static uint8 InverseIfNecessary(uint8 data, Channel::E chan)
{
    if (set.chan[chan].inverse)
    {
        return (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE));
    }
    return data;
}
*/


uint16       ReaderFPGA::data_rel_A[FPGA_MAX_POINTS];
uint16       ReaderFPGA::data_rel_B[FPGA_MAX_POINTS];
int          ReaderFPGA::addition_shift = 0;
DataSettings ReaderFPGA::ds;


void ReaderFPGA::Read(bool necessary_shift, bool save_to_storage)
{
    FPGA::in_processing_of_read = true;
    if (static_cast<TBase::E>(ds.tBase) < TBase::_100ns)
    {
        ReadRandomizeMode();
    }
    else
    {
        ReadRealMode(necessary_shift);
    }

    static uint prevTime = 0;

    if (save_to_storage || (TIME_MS - prevTime > 500))
    {
        prevTime = TIME_MS;
        if (!sTime_RandomizeModeEnabled())
        {
            InverseDataIsNecessary(ChA, data_rel_A);
            InverseDataIsNecessary(ChB, data_rel_B);
        }

        Storage::AddData(data_rel_A, data_rel_B, ds);

        if (TRIG_MODE_FIND_IS_AUTO && TrigLev::need_auto_find)
        {
            TrigLev::FindAndSet();
        }
    }

    FPGA::in_processing_of_read = false;
}


void ReaderFPGA::ReadRandomizeMode()
{
    int Tsm = CalculateShift();
    if (Tsm == TShift::NULL_VALUE)
    {
        return;
    };

    int step = FPGA::Randomizer::Kr[SET_TBASE];
    int index = Tsm - step - addition_shift;

    if (index < 0)
    {
        index += step;        // WARN
    }

    uint16 *pData0 = &data_rel_A[index];
    uint16 *const pData0Last = &data_rel_A[FPGA_MAX_POINTS - 1];
    uint16 *pData1 = &data_rel_B[index];
    uint16 *const pData1Last = &data_rel_B[FPGA_MAX_POINTS - 1];

    uint16 *const first0 = &data_rel_A[0];
    uint16 *const last0 = pData0Last;
    uint16 *const first1 = &data_rel_B[0];
    uint16 *const last1 = pData1Last;

    int numAve = NUM_AVE_FOR_RAND;

    if (ENumAveraging::NumAverages() > numAve)
    {
        numAve = ENumAveraging::NumAverages();
    }

    int addShiftMem = step / 2;

    if (START_MODE_IS_SINGLE || SAMPLE_TYPE_IS_REAL)
    {
        ClearData();
    }

    while (pData0 < &data_rel_A[FPGA_MAX_POINTS])
    {
        //        volatile uint16 data10 = *RD_ADC_B2; //-V2551
                //uint8 data11 = *RD_ADC_B1;
        //        volatile uint16 data00 = *RD_ADC_A2; //-V2551
                //uint8 data01 = *RD_ADC_A1;

                /*
                if (*pData0 == 0 || numAve == 1 || startMode == StartMode::Single)
                {
                */
        if (pData0 >= first0 && pData0 <= last0)
        {
            WRITE_AND_OR_INVERSE(pData0, data00, ChA);
        }

        uint16 *addr = pData0 + addShiftMem;
        if (addr >= first0 && addr <= last0)
        {
            //                WRITE_AND_OR_INVERSE(addr, data01, ChA);
        }

        if (pData1 >= first1 && pData1 <= last1)
        {
            WRITE_AND_OR_INVERSE(pData1, data10, ChB);
        }
        addr = pData1 + addShiftMem;
        if (addr >= first1 && addr <= last1)
        {
            //                WRITE_AND_OR_INVERSE(addr, data11, ChB);
        }
        /*
        }
        else
        {
            if (pData0 >= first0 && pData0 <= last0)
            {
                *pData0 = (float)(numAve - 1) / (float)(numAve)* (*pData0) + InverseIfNecessary(data00, ChA) * 1.0F / (float)numAve;
            }

            uint8 *addr = pData0 + addShiftMem;
            if (addr >= first0 && addr <= last0)
            {
                *addr = (float)(numAve - 1) / (float)(numAve)* (*(pData0 + addShiftMem)) + InverseIfNecessary(data01, ChA) * 1.0F / (float)numAve;
            }

            if (pData1 >= first1 && pData1 <= last1)
            {
                *pData1 = (float)(numAve - 1) / (float)(numAve)* (*pData1) + InverseIfNecessary(data10, ChB) * 1.0F / (float)numAve;
            }

            addr = pData1 + addShiftMem;

            if (addr >= first1 && addr <= last1)
            {
                *addr = (float)(numAve - 1) / (float)(numAve)* (*(pData1 + addShiftMem)) + InverseIfNecessary(data11, ChB) * 1.0F / (float)numAve;
            }
        }
        */

        pData0 += step;
        pData1 += step;
    }

    if (START_MODE_IS_SINGLE || SAMPLE_TYPE_IS_REAL)
    {
        Processing::InterpolationSinX_X(data_rel_A, SET_TBASE);
        Processing::InterpolationSinX_X(data_rel_B, SET_TBASE);
    }
}


void ReaderFPGA::ReadRealMode(bool necessary_shift)
{
    uint16 *p0 = &data_rel_A[0];
    uint16 *p1 = &data_rel_B[0];
    uint16 *endP = &data_rel_A[FPGA_MAX_POINTS];

    if (ds.peakDet != PeackDetMode::Disable)
    {
        uint16 *p0min = p0;
        uint16 *p0max = p0min + 512;
        uint16 *p1min = p1;
        uint16 *p1max = p1min + 512;
        while ((p0max < endP) && FPGA::in_processing_of_read)
        {
            uint16 data = *RD_ADC_B;
            *p1max++ = data;
            data = *RD_ADC_B;
            *p1min++ = data;
            data = *RD_ADC_A;
            *p0min++ = data;
            data = *RD_ADC_A;
            *p0max++ = data;
        }
    }
    else
    {
        while ((p0 < endP) && FPGA::in_processing_of_read)
        {
            *p1++ = *RD_ADC_B;
            *p1++ = *RD_ADC_B;
            uint16 data = *RD_ADC_A;
            *p0++ = data;
            data = *RD_ADC_A;
            *p0++ = data;
        }

        int shift = 0;
        if (SET_TBASE == TBase::_100ns || SET_TBASE == TBase::_200ns)
        {
            shift = CalculateShift();
        }
        else if (necessary_shift)
        {
            //shift = set.debug.altShift;       WARN Остановились на жёстком задании дополнительного смещения. На PageDebug выбор закомментирован, можно включить при необходимости
            shift = -1;
        }
        if (shift != 0)
        {
            if (shift < 0)
            {
                shift = -shift;
                for (int i = FPGA_MAX_POINTS - shift - 1; i >= 0; i--)
                {
                    data_rel_A[i + shift] = data_rel_A[i];
                    data_rel_B[i + shift] = data_rel_B[i];
                }
            }
            else
            {
                for (int i = shift; i < FPGA_MAX_POINTS; i++)
                {
                    data_rel_A[i - shift] = data_rel_A[i];
                    data_rel_B[i - shift] = data_rel_B[i];
                }
            }
        }
    }
}


void ReaderFPGA::ClearData()
{
    std::memset(data_rel_A, 0, FPGA_MAX_POINTS);
    std::memset(data_rel_B, 0, FPGA_MAX_POINTS);
}


void ReaderFPGA::ReadPoint()
{
    FPGA::flag.Read();

    if (FPGA::flag.IsPointReady())
    {
        uint16 dataB1 = *RD_ADC_B;
        uint16 dataB2 = *RD_ADC_B;
        uint16 dataA1 = *RD_ADC_A;
        uint16 dataA2 = *RD_ADC_A;
        Display::AddPoints(dataA2, dataA1, dataB2, dataB1);
    }
}


void ReaderFPGA::InverseDataIsNecessary(Channel::E chan, uint16 *data)
{
    if (SET_INVERSE(chan))
    {
        for (int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            data[i] = (uint8)((int)(2 * AVE_VALUE) - Math::Limitation<uint8>((uint8)data[i], MIN_VALUE, MAX_VALUE));
        }
    }
}


int ReaderFPGA::CalculateShift()            // \todo Не забыть восстановить функцию
{
    uint16 rand = HAL_ADC3::GetValue();
    //LOG_WRITE("rand = %d", (int)rand);
    uint16 min = 0;
    uint16 max = 0;

    if (SET_TBASE == TBase::_200ns)
    {
        return rand < 3000 ? 0 : -1;    // set.debug.altShift; \todo Остановились на жёстком задании дополнительного смещения. На PageDebug выбор 
                                        // закомментирован, можно раскомментировать при необходимости
    }

    if (!FPGA::Randomizer::CalculateGate(rand, &min, &max))
    {
        return TShift::NULL_VALUE;
    }

    //LOG_WRITE("ворота %d - %d", min, max);

    //min += 100;
    //max -= 100;

    if (sTime_RandomizeModeEnabled())
    {
        float tin = static_cast<float>(rand - min) / (max - min) * 10e-9F;
        int retValue = static_cast<int>(tin / 10e-9F * FPGA::Randomizer::Kr[SET_TBASE]);
        return retValue;
    }

    if (SET_TBASE == TBase::_100ns && rand < (min + max) / 2)
    {
        return 0;
    }

    return -1;  // set.debug.altShift;      \todo Остановились на жёстком задании дополнительного смещения. На PageDebug выбор закомментирован, 
                                            //можно раскомментировать при необходимости
}
