// 2021/03/16 16:35:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
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


const uint16 *const addresses_ADC[2] = { RD_ADC_A, RD_ADC_B };


void ReaderFPGA::ReadData()
{
    mutex_read.Lock();

    DataReadingKeeper data;

    uint16 addr_read = CalculateAddressRead();

    if (TBase::IsRandomize())
    {
        Read::Randomizer::Channel(*data.data, ChA, addr_read);
        Read::Randomizer::Channel(*data.data, ChB, addr_read);
    }
    else
    {
        Read::Real::Channel(*data.data, ChA, addr_read);
        Read::Real::Channel(*data.data, ChB, addr_read);
    }

    Storage::Append(*data.data);

    mutex_read.Unlock();
}


void ReaderFPGA::Read::Real::Channel(DataReading &data, const ::Channel &ch, uint16 addr_read)
{
    FPGA::BUS::Write(WR_PRED, addr_read, false);
    FPGA::BUS::Write(WR_ADDR_READ, 0xffff, false);

    uint16 *p = (uint16 *)data.Data(ch);
    uint16 *end = (uint16 *)(data.Data(ch) + data.Settings().BytesInChannel());

    volatile const uint16 * const address = ADDRESS_READ(ch);

    int counter = 0;

    while (p < end)
    {
        *p++ = *address;
        counter++;
    }
}


bool ReaderFPGA::Read::Randomizer::IndexFirstPoint(int *first_out, int *skipped_out)
{
    Int Tsm = CalculateShift();

    if (!Tsm.IsValid())
    {
        return false;
    }

    int step = TBase::StepRand();

    static const int NUM_ADD_STEPS = 2;

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


void ReaderFPGA::Read::Randomizer::Channel(DataReading &dr, const ::Channel &ch, uint16 addr_read)
{
    int index = 0;
    int num_skipped = 0;

    if (!IndexFirstPoint(&index, &num_skipped))
    {
        return;
    }

    DataSettings &ds = dr.Settings();

    uint bytes_in_channel = ds.BytesInChannel();

    const uint16 *const address = ADDRESS_READ(ch);
    uint8 *addr_wr = dr.Data(ch);                           // Сюда будем писать считываемые данные
    const uint8 * const addr_wr_last = addr_wr + bytes_in_channel;

    if (ds.is_clean == 1)
    {
        std::memset(addr_wr, Value::NONE, bytes_in_channel);
        ds.is_clean = 0;
    }

    UtilizeFirstBytes(address, num_skipped);

    addr_wr += index;

    FPGA::BUS::Write(WR_PRED, addr_read, false);
    FPGA::BUS::Write(WR_ADDR_READ, 0xffff, false);

    uint num_bytes = bytes_in_channel / TBase::StepRand();

    for (uint i = 0; i < num_bytes; i++)
    {
        if (addr_wr >= dr.Data(ch) && addr_wr < addr_wr_last)
        {
            *addr_wr = (uint8)*address;
        }

        addr_wr += TBase::StepRand();
    }
}


void ReaderFPGA::Read::Randomizer::UtilizeFirstBytes(const uint16 * const address, int num_words)
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


void ReaderFPGA::InverseDataIsNecessary(DataReading &data, const Channel &ch)
{
    if (ch.IsInversed())
    {
        uint8 *d = data.Data(ch);
        uint size = data.Settings().BytesInChannel();

        for (uint i = 0; i < size; i++)
        {
            d[i] = (uint8)((int)(2 * Value::AVE) - Math::Limitation<uint8>((uint8)d[i], Value::MIN, Value::MAX));
        }
    }
}


Int ReaderFPGA::CalculateShift()
{
    Uint16 rand = HAL_ADC1::GetValue();

    if (rand.IsValid())
    {
        uint16 min = 0;
        uint16 max = 0;

        if (FPGA::Randomizer::CalculateGate(rand, &min, &max))
        {
            float tin = (float)(rand - min) / (max - min) * 10e-9F;

            return (int)(tin / 10e-9F * TBase::StepRand());
        }
    }
    else
    {
        LOG_WRITE("Невалидный ADC");
    }

    return InvalidInt();
}


uint16 ReaderFPGA::CalculateAddressRead()
{
    uint shift = TBase::IsRandomize() ?
        set.memory.enum_points_fpga.BytesInChannel() / TBase::StepRand() :
        set.memory.enum_points_fpga.BytesInChannel() / 2;

    uint result = HAL_FMC::Read(RD_ADDR_LAST_RECORD) - shift;

    return (uint16)(result + LaunchFPGA::DeltaReadAddress());
}
