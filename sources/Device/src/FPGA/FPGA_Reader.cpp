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


uint16       ReaderFPGA::data_a[FPGA_MAX_POINTS];
uint16       ReaderFPGA::data_b[FPGA_MAX_POINTS];
int          ReaderFPGA::addition_shift = 0;
DataSettings ReaderFPGA::ds;


void ReaderFPGA::ReadData(bool necessary_shift, bool save_to_storage)
{
    FPGA::in_processing_of_read = true;

    if (FPGA_IN_RANDOMIZE_MODE)
    {
        ReadRandomizeMode();
    }
    else
    {
        ReadRealMode(necessary_shift);
    }

    SaveToStorage(save_to_storage);

    FPGA::in_processing_of_read = false;
}


void ReaderFPGA::SaveToStorage(bool save_to_storage)
{
    static uint prevTime = 0;

    if (save_to_storage || (TIME_MS - prevTime > 500))
    {
        prevTime = TIME_MS;

        if (!sTime_RandomizeModeEnabled())
        {
            InverseDataIsNecessary(ChA, data_a);
            InverseDataIsNecessary(ChB, data_b);
        }

        Storage::AddData(data_a, data_b, ds);

        if (TRIG_MODE_FIND_IS_AUTO && TrigLev::need_auto_find)
        {
            TrigLev::FindAndSet();
        }
    }
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

    uint16 *pData0 = &data_a[index];
    uint16 *const pData0Last = &data_a[FPGA_MAX_POINTS - 1];
    uint16 *pData1 = &data_b[index];
    uint16 *const pData1Last = &data_b[FPGA_MAX_POINTS - 1];

    uint16 *const first0 = &data_a[0];
    uint16 *const last0 = pData0Last;
    uint16 *const first1 = &data_b[0];
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

    while (pData0 < &data_a[FPGA_MAX_POINTS])
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
        Processing::InterpolationSinX_X(data_a, SET_TBASE);
        Processing::InterpolationSinX_X(data_b, SET_TBASE);
    }
}


void ReaderFPGA::ReadRealMode(bool /*necessary_shift*/)
{
    if (ds.peakDet != PeackDetMode::Disable)
    {
        ReadRealModePeakDetOn();
    }
    else
    {
        ReadRealModePeakDetOff();
    }
}


void ReaderFPGA::ReadRealModePeakDetOn()
{
    uint16 *a = &data_a[0];
    uint16 *b = &data_b[0];
    uint16 *end = &data_a[FPGA_MAX_POINTS];

    uint16 *a_min = a;
    uint16 *a_max = a_min + 512;
    uint16 *b_min = b;
    uint16 *b_max = b_min + 512;

    while ((a_max < end) && FPGA::in_processing_of_read)
    {
        uint16 data = *RD_ADC_B;
        *b_max++ = data;
        data = *RD_ADC_B;
        *b_min++ = data;
        data = *RD_ADC_A;
        *a_min++ = data;
        data = *RD_ADC_A;
        *a_max++ = data;
    }
}


void ReaderFPGA::ReadRealModePeakDetOff()
{
    uint16 addr_stop = ReadAddressStop();

    uint16 *a = &data_a[0];
    uint16 *b = &data_b[0];
    uint16 *end = &data_a[FPGA_MAX_POINTS];

    while ((a < end) && FPGA::in_processing_of_read)
    {
        *b++ = *RD_ADC_B;
        *b++ = *RD_ADC_B;

        *a++ = *RD_ADC_A;
        *a++ = *RD_ADC_A;
    }
}


void ReaderFPGA::ClearData()
{
    std::memset(data_a, 0, FPGA_MAX_POINTS);
    std::memset(data_b, 0, FPGA_MAX_POINTS);
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


int ReaderFPGA::CalculateShift()            // \todo �� ������ ������������ �������
{
    uint16 rand = HAL_ADC3::GetValue();
    //LOG_WRITE("rand = %d", (int)rand);
    uint16 min = 0;
    uint16 max = 0;

    if (SET_TBASE == TBase::_200ns)
    {
        return rand < 3000 ? 0 : -1;    // set.debug.altShift; \todo ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� 
                                        // ���������������, ����� ����������������� ��� �������������
    }

    if (!FPGA::Randomizer::CalculateGate(rand, &min, &max))
    {
        return TShift::NULL_VALUE;
    }

    //LOG_WRITE("������ %d - %d", min, max);

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

    return -1;  // set.debug.altShift;      \todo ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� ���������������, 
                                            //����� ����������������� ��� �������������
}


uint16 ReaderFPGA::ReadAddressStop()
{
    return (uint16)(*RD_ADDR_NSTOP + 16384 - SET_POINTS_IN_CHANNEL / 2 - 1 - FPGA::add_N_stop);
}
