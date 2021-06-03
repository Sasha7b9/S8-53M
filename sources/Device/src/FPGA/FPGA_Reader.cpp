// 2021/03/16 16:35:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "Display/PainterData.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Reader.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Preparator.h"
#include "Panel/Panel.h"
#include "Utils/ProcessingSignal.h"
#include <cstring>


#define WRITE_AND_OR_INVERSE(addr, data, ch)
/*
    if(SET_INVERSE(ch))                                                                   \
    {                                                                                               \
        data = (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE));    \
    }                                                                                               \
    *addr = (uint16)data;
* */

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


Mutex ReaderFPGA::mutex_read;

const uint16 *ReaderFPGA::ADC::address = nullptr;
int16         ReaderFPGA::ADC::balance = 0;


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
    const uint16 *end = (uint16 *)(data.Data(ch) + data.Settings().BytesInChannel());

    const uint16 *address = ADDRESS_READ(ch);

    ADC::SetParameters(address, ch.value);

    ADC::ReadPoints(p, end);

//    while (p < end)
//    {
//        *p++ = ADC::ReadPoints();
//    }

    if (!PeackDetMode::IsEnabled() && FPGA::flag.IsFirstByte0())
    {
        uint8 *last = data.Data(ch) + data.Settings().BytesInChannel();

        for (uint8 *pointer = data.Data(ch) + 1; pointer < last; pointer++)
        {
            *(pointer - 1) = *pointer;
        }
    }
}

 
void ReaderFPGA::ADC::ReadPoints(uint16 *first, const uint16 *last)
{
    while (first < last)
    {
        *first++ = ReadPoints();
    }
}


uint16 ReaderFPGA::ADC::ReadPoints()
{
    BitSet16 data;
    data.half_word = *address;

    int16 byte1 = data.byte1;
    Math::AddLimitation<int16>(&byte1, balance, 0, 255);
    data.byte1 = (uint8)byte1;

    return data.half_word;
}


void ReaderFPGA::ADC::SetParameters(const uint16 *_address, Channel::E ch)
{
    address = _address;
    balance = setNRST.chan[ch].balance_ADC;
}


bool ReaderFPGA::Read::Randomizer::IndexFirstPoint(int *first_out, int *skipped_out)
{
    Int Tsm = CalculateShift();

    if (!Tsm.IsValid())
    {
        return false;
    }

    int step = TBase::StepRand();

    int index = Tsm - 2 * step;

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


uint8 *ReaderFPGA::Read::Randomizer::CalculateFirstAddressWrite(DataReading &dr, const ::Channel &ch)
{
    int index = 0;
    int num_skipped = 0;

    if (!IndexFirstPoint(&index, &num_skipped))
    {
        return nullptr;
    }

    DataSettings &ds = dr.Settings();

    uint8 *awrite = dr.Data(ch);                                           // Сюда будем писать считываемые данны

    if (ds.is_clean == 1)
    {
        std::memset(awrite, Value::NONE, ds.BytesInChannel());
        ds.is_clean = 0;
    }

    UtilizeFirstBytes(ADDRESS_READ(ch), num_skipped);

    if (awrite != nullptr)
    {
        awrite += index + LaunchFPGA::AdditionalOffsetIndexFirst();
    }

    return awrite;
}


void ReaderFPGA::Read::Randomizer::Channel(DataReading &dr, const ::Channel &ch, uint16 addr_read)
{
    uint8 *awrite = CalculateFirstAddressWrite(dr, ch);       // Первый адрес сохранения читаемых данных

    if (awrite == nullptr)
    {
        return;
    }

    FPGA::BUS::Write(WR_PRED, addr_read, false);
    FPGA::BUS::Write(WR_ADDR_READ, 0xffff, false);

    uint bytesInChannel = dr.Settings().BytesInChannel();
    uint numBytes = bytesInChannel / TBase::StepRand();

    const uint8 *awriteLast = awrite + bytesInChannel;

    if (awriteLast > dr.DataEnd(ch))
    {
        awriteLast = dr.DataEnd(ch);
    }

    const uint16 *const aread = ADDRESS_READ(ch);

    for (uint i = 0; i < numBytes; i++)
    {
        if (awrite >= dr.Data(ch) && awrite < awriteLast)
        {
            *awrite = (uint8)*aread;
        }

        awrite += TBase::StepRand();
    }
}


void ReaderFPGA::Read::Randomizer::UtilizeFirstBytes(const uint16 * const address, int num_words)
{
    volatile uint16 data;

    for (int i = 0; i < num_words; i++)
    {
        data = *address;
    }

#ifdef __linux__
    data = data;
#endif
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


// Возвращает true, если нужно отбросить значение рандомизатора
static bool NeedToDiscardValueRandomizer(uint16 rand, uint16 min, uint16 max)
{
    if (rand > max - setNRST.rand.gate_max * 10)
    {
        return true;
    }

    if (rand < min + setNRST.rand.gate_max * 10)
    {
        return true;
    }

    return false;
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
            if (!NeedToDiscardValueRandomizer(rand, min, max))
            {
                float tin = (float)(rand - min) / (max - min) * 10e-9F;

                return (int)(tin / 10e-9F * TBase::StepRand());
            }
        }
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
