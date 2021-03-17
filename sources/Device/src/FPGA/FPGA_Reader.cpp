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


uint8        ReaderFPGA::data_a[FPGA_MAX_POINTS];
uint8        ReaderFPGA::data_b[FPGA_MAX_POINTS];
int          ReaderFPGA::addition_shift = 0;
DataSettings ReaderFPGA::ds;


uint16 *addresses_ADC[2] = { RD_ADC_A, RD_ADC_B };


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
}


void ReaderFPGA::ReadRealModePeakDetOff()
{
    uint16 addr_stop = ReadAddressStop();

    ReadChannel(data_a, ChA, addr_stop);

    ReadChannel(data_b, ChB, addr_stop);
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


void ReaderFPGA::InverseDataIsNecessary(Channel::E chan, uint8 *data)
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


uint16 ReaderFPGA::ReadAddressStop()
{
    return (uint16)(*RD_ADDR_NSTOP + 16384 - SET_POINTS_IN_CHANNEL / 2 - 1 - FPGA::add_N_stop);
}


void ReaderFPGA::ReadChannel(uint8 *data, Channel::E ch, uint16 addr_stop)
{
    *WR_PRED = addr_stop;
    *WR_ADDR_STOP = 0xffff;

    uint16 *p = (uint16 *)data;
    uint16 *end = (uint16 *)(data + SET_POINTS_IN_CHANNEL);

    uint16 *address = ADDRESS_READ(ch);

    addr_stop = *address;

    while (p < end && FPGA::in_processing_of_read)
    {
        *p++ = *address;
        *p++ = *address;
        *p++ = *address;
        *p++ = *address;
        *p++ = *address;
        *p++ = *address;
        *p++ = *address;
        *p++ = *address;
    }
}
