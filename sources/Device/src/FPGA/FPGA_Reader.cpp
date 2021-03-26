// 2021/03/16 16:35:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Log_.h"
#include "common/Utils/Math_.h"
#include "Display/PainterData.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Preparator.h"
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
static uint8 InverseIfNecessary(uint8 data, Channel::E ch)
{
    if (set.chan[ch].inverse)
    {
        return (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE));
    }
    return data;
}
*/


int   ReaderFPGA::addition_shift = 0;
Mutex ReaderFPGA::mutex_read;


uint16 *addresses_ADC[2] = { RD_ADC_A, RD_ADC_B };


void ReaderFPGA::ReadData()
{
    mutex_read.Lock();

    DataReadingKeeper data;

    if (data.data->Settings().IsEnabledPeakDet())
    {
        Read::PeakDetOn(*data.data);
    }
    else
    {
        Read::PeakDetOff(*data.data);
    }

    Storage::Append(*data.data);

    mutex_read.Unlock();
}


void ReaderFPGA::Read::PeakDetOn(DataReading &)
{
}


void ReaderFPGA::Read::PeakDetOff(DataReading &data)
{
    uint16 addr_stop = ReadAddressStop();

    if (TBase::IsRandomize())
    {
        Randomizer::Channel(data, ChA, addr_stop);
        Randomizer::Channel(data, ChB, addr_stop);
    }
    else
    {
        Real::Channel(data, ChA, addr_stop);
        Real::Channel(data, ChB, addr_stop);
    }
}


void ReaderFPGA::Read::Real::Channel(DataReading &data, const ::Channel &ch, uint16 addr_stop)
{
    *WR_PRED = addr_stop;
    *WR_ADDR_STOP = 0xffff;

    uint16 *p = (uint16 *)data.Data(ch);
    uint16 *end = (uint16 *)(data.Data(ch) + data.Settings().BytesInChannel());

    volatile uint16 *address = ADDRESS_READ(ch);

    while (p < end)
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


#define NUM_ADD_STEPS 2


bool ReaderFPGA::Read::Randomizer::IndexFirstPoint(int *first_out, int *skipped_out)
{
    Int Tsm = CalculateShift();

    if (!Tsm.IsValid())
    {
        return false;
    }

    int step = TBase::StepRand();

    int index = Tsm - addition_shift - NUM_ADD_STEPS * step;

    int num_skipped = 0;

    while (index < 0)
    {
        index += step;
        num_skipped++;
    }

    *first_out = index;
    *skipped_out = num_skipped;

    return true;
}


void ReaderFPGA::Read::Randomizer::Channel(DataReading &dr, const ::Channel &ch, uint16 addr_stop)
{
    int index = 0;
    int num_skipped = 0;

    if (!IndexFirstPoint(&index, &num_skipped))
    {
        return;
    }

    LOG_WRITE("index = %d", index);

    uint bytes_in_channel = dr.Settings().BytesInChannel();

    uint16 *address = ADDRESS_READ(ch);
    uint8 *data = dr.Data(ch);
    uint8 *last = data + bytes_in_channel;
    std::memset(data, Value::NONE, bytes_in_channel);

    UtilizeFirstBytes(address, num_skipped);

    data += index;

    uint16 addr_first_read =
        (uint16)(addr_stop + 16384 - (uint16)(bytes_in_channel / TBase::StepRand()) - 1 - NUM_ADD_STEPS);

    HAL_FMC::Write(WR_PRED, addr_first_read);
    HAL_FMC::Write(WR_ADDR_STOP, 0xffff);

    while (data < last)
    {
        *data = (uint8)*address;
        data += TBase::StepRand();
    }
}


void ReaderFPGA::Read::Randomizer::UtilizeFirstBytes(uint16 *address, int num_words)
{
    __IO uint16 data;

    for (int i = 0; i < num_words; i++)
    {
        data = *address;
    }
}


void ReaderFPGA::ReadPoint()
{
    FPGA::flag.Read();

    if (FPGA::flag.IsPointReady())
    {
//        uint16 dataB1 = *RD_ADC_B;
//        uint16 dataB2 = *RD_ADC_B;
//        uint16 dataA1 = *RD_ADC_A;
//        uint16 dataA2 = *RD_ADC_A;

//        PainterData::AddPoints(dataA2, dataA1, dataB2, dataB1);
    }
}


void ReaderFPGA::InverseDataIsNecessary(const Channel &ch, uint8 *data)
{
    if (ch.IsInversed())
    {
        for (int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            data[i] = (uint8)((int)(2 * Value::AVE) - Math::Limitation<uint8>((uint8)data[i], Value::MIN, Value::MAX));
        }
    }
}


Int ReaderFPGA::CalculateShift()
{
    uint16 rand = HAL_ADC1::GetValue();

    uint16 min = 0;
    uint16 max = 0;

    if (FPGA::Randomizer::CalculateGate(rand, &min, &max))
    {
        float tin = (float)(rand - min) / (max - min) * 10e-9F;

        return (int)(tin / 10e-9F * TBase::StepRand());
    }

    return InvalidInt();
}


uint16 ReaderFPGA::ReadAddressStop()
{
    return (uint16)(*RD_ADDR_NSTOP + 16384 - FPGA::SET::PointsInChannel() / 2 - 1 - FPGA::add_N_stop);
}
