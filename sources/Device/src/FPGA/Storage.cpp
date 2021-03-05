#include "defines.h"
#include "common/Log_.h"
#include "common/Utils/Math_.h"
#include "FPGA/DataSettings.h"
#include "FPGA/Storage.h"
#include "Settings/Settings.h"
#include <cstring>

uint8  Storage::pool[SIZE_POOL] = {0};
uint8 *Storage::beginPool = &(pool[0]);
uint8 *Storage::endPool = &(pool[SIZE_POOL - 1]);
int    Storage::allData = 0;
float  Storage::aveData0[FPGA_MAX_POINTS] = {0.0F};
float  Storage::aveData1[FPGA_MAX_POINTS] = {0.0F};
bool   Storage::newSumCalculated[NumChannels] = {true, true};
uint   Storage::sum[NumChannels][FPGA_MAX_POINTS];
uint8  Storage::limitUp[NumChannels][FPGA_MAX_POINTS];
uint8  Storage::limitDown[NumChannels][FPGA_MAX_POINTS];
DataSettings *Storage::firstElem = 0;
DataSettings *Storage::lastElem = 0;

uint8 *Storage::dataA = nullptr;
uint8 *Storage::dataB = nullptr;
DataSettings *Storage::set = nullptr;

uint8 *Storage::dataIntA = nullptr;
uint8 *Storage::dataIntB = nullptr;
DataSettings *Storage::dsInt = nullptr;

uint8 *Storage::dataLastA = nullptr;
uint8 *Storage::dataLastB = nullptr;
DataSettings *Storage::dsLast = nullptr;


void Storage::PrintElement(DataSettings *dp)
{
    if(dp == 0)
    {
        LOG_WRITE("Нулевой элемент");
    }
    else
    {
        LOG_WRITE("addr:%x, addrNext:%x, addrPrev:%x, size:%d", dp, dp->addrNext, dp->addrPrev, SizeElem(dp));
    }
}


void Storage::Clear()
{
    firstElem = 0;
    lastElem = (DataSettings*)beginPool;
    lastElem->addrNext = 0;
    lastElem->addrPrev = 0;
    ClearLimitsAndSums();
}


void Storage::ClearLimitsAndSums()
{
    std::memset(limitUp[0], 0, FPGA_MAX_POINTS);
    std::memset(limitUp[1], 0, FPGA_MAX_POINTS);
    std::memset(limitDown[0], 0xff, FPGA_MAX_POINTS);
    std::memset(limitDown[1], 0xff, FPGA_MAX_POINTS);
    std::memset(&(sum[0][0]), 0, NumChannels * FPGA_MAX_POINTS * sizeof(uint));
}


void Storage::CalculateAroundAverage(uint16 *data0, uint16 *data1, const DataSettings *dss)
{
    int numAveData = NumElementsWithCurrentSettings();
    int size = (int)(dss->length1channel * (dss->peakDet == PeackDetMode::Disable ? 1 : 2));
    if (numAveData == 1)
    {
        for (int i = 0; i < size; i++)
        {
            aveData0[i] = data0[i]; //-V2563 //-V2564
            aveData1[i] = data1[i]; //-V2563 //-V2564
        }
    }
    else
    {
        if (numAveData > ENumAveraging::NumAverages())
        {
            numAveData = ENumAveraging::NumAverages();
        }
        float numAveDataF = static_cast<float>(numAveData);
        float numAveDataFless = numAveDataF - 1.0F;
        float numAveDataInv = 1.0F / numAveDataF;
        float* aData0 = &aveData0[0];
        float* aData1 = &aveData1[0];
        uint16* d0 = &data0[0]; //-V2563
        uint16* d1 = &data1[0]; //-V2563
        float* endData = &aveData0[size];

        do 
        {
            *aData0++ = ((*aData0) * numAveDataFless + (float)(*d0++)) * numAveDataInv; //-V567
            *aData1++ = ((*aData1) * numAveDataFless + (float)(*d1++)) * numAveDataInv; //-V567
        } while (aData0 != endData);
    }
}


void Storage::AddData(uint16 *data0, uint16 *data1, DataSettings dss)
{
    dss.time = HAL_RTC::GetPackedTime();

    if(dss.enableCh0 == 0 && dss.enableCh1 == 0)
    {
        return;
    }
    CalculateLimits(data0, data1, &dss);

    PushData(&dss, data0, data1);

    CalculateSums();

    CalculateAroundAverage(data0, data1, &dss);
    allData++;
}


int Storage::AllDatas()
{
    return allData;
}


void Storage::CalculateLimits(uint16 * data0, uint16 * data1, const DataSettings *dss)
{
    uint numElements = dss->length1channel * (dss->peakDet == PeackDetMode::Disable ? 1 : 2);

    if(NumElementsInStorage() == 0 || NUM_MIN_MAX == 1 || (!SettingsIsEquals(dss, GetSettingsDataFromEnd(0))))
    {
        for(uint i = 0; i < numElements; i++)
        {
            limitDown[0][i] = (uint8)data0[i]; //-V2563
            limitUp[0][i] = (uint8)data0[i]; //-V2563
            limitDown[1][i] = (uint8)data1[i]; //-V2563
            limitUp[1][i] = (uint8)data1[i]; //-V2563
        }
    }
    else
    {
        int allDatas = NumElementsWithSameSettings();
        LIMITATION(allDatas, allDatas, 1, NUM_MIN_MAX); //-V2516
        
        if(NumElementsWithSameSettings() >= NUM_MIN_MAX)
        {
            for(uint i = 0; i < numElements; i++)
            {
                limitDown[0][i] = (uint8)data0[i]; //-V2563
                limitUp[0][i] = (uint8)data0[i]; //-V2563
                limitDown[1][i] = (uint8)data1[i]; //-V2563
                limitUp[1][i] = (uint8)data1[i]; //-V2563
            }
            allDatas--;
        }
     
        for(int numData = 0; numData < allDatas; numData++)
        {
            pUCHAR dA = GetData(Channel::A, numData);
            pUCHAR dB = GetData(Channel::B, numData);
            for(uint i = 0; i < numElements; i++)
            {
                if(dA[i] < limitDown[0][i])  { limitDown[0][i] = dA[i]; } //-V2563
                if(dA[i] > limitUp[0][i])    { limitUp[0][i] = dA[i];   } //-V2563
                if(dB[i] < limitDown[1][i])  { limitDown[1][i] = dB[i]; } //-V2563
                if(dB[i] > limitUp[1][i])    { limitUp[1][i] = dB[i];   } //-V2563
            }
        }
    }
}


void Storage::CalculateSums()
{
    DataSettings *ds = 0;
    uint8 *data0 = 0;
    uint8 *data1 = 0;

    GetDataFromEnd(0, &ds, &data0, &data1);
    
    uint numPoints = ds->length1channel * (ds->peakDet == PeackDetMode::Disable ? 1 : 2);

    int numAveragings = 0;

    if (sTime_RandomizeModeEnabled())
    {
        numAveragings = NUM_AVE_FOR_RAND;
    }

    if (ENumAveraging::NumAverages() > numAveragings)
    {
        numAveragings = ENumAveraging::NumAverages();
    }

    for(uint i = 0; i < numPoints; i++)
    {
        sum[0][i] = data0[i]; //-V2563
        sum[1][i] = data1[i]; //-V2563
    }
    if(numAveragings > 1)
    {
        int numSameSettins = NumElementsWithSameSettings();
        if(numSameSettins < numAveragings)
        {
            numAveragings = numSameSettins;
        }
        for(int i = 1; i < numAveragings; i++)
        {
            GetDataFromEnd(i, &ds, &data0, &data1);
            for(uint point = 0; point < numPoints; point++)
            {
                sum[0][point] += data0[point]; //-V2563
                sum[1][point] += data1[point]; //-V2563
            }
        }
    }

    newSumCalculated[0] = true;
    newSumCalculated[1] = true;
}


int Storage::NumElementsWithSameSettings()
{
    int retValue = 0;
    int numElements = NumElementsInStorage();
    for(retValue = 1; retValue < numElements; retValue++)
    {
        if(!SettingsIsIdentical(retValue, retValue - 1))
        {
            break;
        }
    }
    return retValue;
}


int Storage::NumElementsWithCurrentSettings()
{
    DataSettings dp;
    FPGA::FillDataPointer(&dp);
    int retValue = 0;
    int numElements = NumElementsInStorage();
    for(retValue = 0; retValue < numElements; retValue++)
    {
        if(!SettingsIsEquals(&dp, FromEnd(retValue)))
        {
            break;
        }
    }
    return retValue;
}


int Storage::NumElementsInStorage()
{
    int retValue = 0;
    DataSettings *elem = firstElem;
    if(firstElem != 0)
    {
        if(firstElem == lastElem)
        {
            retValue = 1;
        }
        else
        {
            retValue++;
            while((elem = NextElem(elem)) != 0) //-V2561
            {
                retValue++;
            }
        }
    }
    return retValue;
}


DataSettings* Storage::GetSettingsDataFromEnd(int fromEnd)
{
    return FromEnd(fromEnd);
}


bool Storage::GetDataFromEnd(int fromEnd, DataSettings **ds, uint8 **data0, uint8 **data1)
{
    static uint8 dataImportRel[2][FPGA_MAX_POINTS];

    DataSettings* dp = FromEnd(fromEnd);
    if(dp == 0)
    {
        return false;
    }

    if(data0 != 0)
    {
        *data0 = CopyData(dp, Channel::A, dataImportRel) ?  &dataImportRel[0][0] : 0;
    }
    if(data1 != 0)
    {
        *data1 = CopyData(dp, Channel::B, dataImportRel) ? &dataImportRel[1][0] : 0;
    }
    *ds = dp;
    
    return true;
}


uint8* Storage::GetData(Channel::E chan, int fromEnd)
{
    static uint8 dataImportRel[2][FPGA_MAX_POINTS];
    DataSettings* dp = FromEnd(fromEnd);
    if(dp == 0)
    {
        return 0;
    }

    return CopyData(dp, chan, dataImportRel) ? &dataImportRel[chan][0] : 0;
}


bool Storage::CopyData(DataSettings *ds, Channel::E chan, uint8 datatImportRel[2][FPGA_MAX_POINTS])
{
    if((chan == Channel::A && ds->enableCh0 == 0) || (chan == Channel::B && ds->enableCh1 == 0))
    {
        return false;
    }
    uint8* pointer = (chan == Channel::A) ? (&datatImportRel[0][0]) : (&datatImportRel[1][0]);

    uint8* address = ((uint8*)ds + sizeof(DataSettings)); //-V2563

    uint length = ds->length1channel * (ds->peakDet == PeackDetMode::Disable ? 1 : 2);

    if(chan == Channel::B && ds->enableCh0 == 1)
    {
        address += length; //-V2563
    }

    std::memcpy(pointer, address, length);

    return true;
}


uint8* Storage::GetAverageData(Channel::E chan)
{
    static uint8 data[NumChannels][FPGA_MAX_POINTS];
    
    if (newSumCalculated[chan] == false)
    {
        return &data[chan][0];
    }

    newSumCalculated[chan] = false;

    DataSettings *ds = 0;
    uint8 *d0, *d1;
    GetDataFromEnd(0, &ds, &d0, &d1);

    if (ds == 0)
    {
        return 0;
    }

    uint numPoints = ds->length1channel * (ds->peakDet == PeackDetMode::Disable ? 1 : 2);

    if (ModeAveraging::Current() == ModeAveraging::Around)
    {
        float *floatAveData = (chan == Channel::A) ? aveData0 : aveData1;
        
        for (uint i = 0; i < numPoints; i++)
        {
            data[chan][i] = (uint8)(floatAveData[i]);
        }
        return &data[chan][0];
    }

    int numAveraging = ENumAveraging::NumAverages();

    LIMIT_ABOVE(numAveraging, NumElementsWithSameSettings());

    for(uint i = 0; i < numPoints; i++)
    {
        data[chan][i] = static_cast<uint8>(sum[chan][i] / numAveraging);
    }

    return &data[chan][0];
}


uint8* Storage::GetLimitation(Channel::E chan, int direction)
{
    uint8 *retValue = 0;

    if(direction == 0)
    {
        retValue = &(limitDown[chan][0]);
    }
    else if(direction == 1) //-V2516
    {
        retValue = &(limitUp[chan][0]);
    }

    return retValue;
}


int Storage::NumberAvailableEntries()
{
    if(firstElem == 0)
    {
        return 0;
    }
    return SIZE_POOL / SizeElem(lastElem);
}


#define COPY_AND_INCREASE(address, data, length)    \
    std::memcpy((address), (data), (length));       \
    address += (length);


void Storage::PushData(DataSettings *dp, uint16 * data0, uint16 * data1)
{
    int required = SizeElem(dp);
    while(MemoryFree() < required)
    {
        RemoveFirstElement();
    }

    uint8* addrRecord = 0;
    if(firstElem == 0)
    {
        firstElem = (DataSettings*)beginPool;
        addrRecord = beginPool;
        dp->addrPrev = 0;
        dp->addrNext = 0;
    }
    else
    {
        addrRecord = (uint8*)lastElem + SizeElem(lastElem); //-V2563
        if(addrRecord + SizeElem(dp) > endPool) //-V2563
        {
            addrRecord = beginPool;
        }
        dp->addrPrev = lastElem;
        lastElem->addrNext = addrRecord;
        dp->addrNext = 0;
    }

    lastElem = (DataSettings*)addrRecord;

    COPY_AND_INCREASE(addrRecord, dp, sizeof(DataSettings)); //-V2563

    uint length = dp->length1channel;

    if(dp->enableCh0 == 1)
    {
        COPY_AND_INCREASE(addrRecord, data0, length); //-V2563
        if (dp->peakDet != PeackDetMode::Disable)
        {
            COPY_AND_INCREASE(addrRecord, data0 + 512, length); //-V2563
        }
    }
    if(dp->enableCh1 == 1)
    {
        COPY_AND_INCREASE(addrRecord, data1, length); //-V2563
        if (dp->peakDet != PeackDetMode::Disable)
        {
            COPY_AND_INCREASE(addrRecord, data1 + 512, length); //-V2563
        }
    }
}


int Storage::MemoryFree()
{
    if (firstElem == 0)
    {
        return SIZE_POOL;
    }
    else if (firstElem == lastElem)
    {
        return (endPool - (uint8*)firstElem - (int)SizeElem(firstElem));
    }
    else if (firstElem < lastElem)
    {
        if ((uint8*)firstElem == beginPool)
        {
            return (endPool - (uint8*)lastElem - SizeElem(lastElem));
        }
        else
        {
            return (uint8*)firstElem - beginPool;
        }
    }
    else if (lastElem < firstElem) //-V2516
    {
        return (uint8*)firstElem - (uint8*)lastElem - SizeElem(lastElem);
    }
    return 0;
}


int Storage::SizeElem(const DataSettings *dp)
{
    int retValue = sizeof(DataSettings);
    if(dp->enableCh0 == 1)
    {
        retValue += dp->length1channel;
        if (dp->peakDet != PeackDetMode::Disable)
        {
            retValue += dp->length1channel;
        }
    }
    if(dp->enableCh1 == 1)
    {
        retValue += dp->length1channel;
        if (dp->peakDet != PeackDetMode::Disable)
        {
            retValue += dp->length1channel;
        }
    }
    return retValue;
}


void Storage::RemoveFirstElement()
{
    firstElem = NextElem(firstElem);
    firstElem->addrPrev = 0;
    allData--;
}


DataSettings* Storage::NextElem(DataSettings *elem)
{
    return (DataSettings*)elem->addrNext;
}


DataSettings* Storage::FromEnd(int indexFromEnd)
{
    if(firstElem == 0)
    {
        return 0;
    }
    int index = indexFromEnd;
    DataSettings *retValue = lastElem;
    while(index != 0 && ((retValue = (DataSettings *)retValue->addrPrev) != 0)) //-V2561
    {
        index--;
    }
    if(index != 0)
    {
        LOG_ERROR("Неправильный индекс %d, всего данных %d", indexFromEnd, AllDatas());
        return 0;
    }
    return retValue;
}


bool Storage::SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1)
{
    DataSettings* dp0 = FromEnd(elemFromEnd0);
    DataSettings* dp1 = FromEnd(elemFromEnd1);
    return SettingsIsEquals(dp0, dp1);
}


bool Storage::SettingsIsEquals(const DataSettings *dp0, const DataSettings *dp1)
{
    bool retValue = (dp0->enableCh0  == dp1->enableCh0) &&
        (dp0->enableCh1     == dp1->enableCh1) &&
        (dp0->inverseCh0    == dp1->inverseCh0) &&
        (dp0->inverseCh1    == dp1->inverseCh1) &&
        (dp0->range[0]      == dp1->range[0]) &&
        (dp0->range[1]      == dp1->range[1]) &&
        (dp0->rShiftCh0     == dp1->rShiftCh0) &&
        (dp0->rShiftCh1     == dp1->rShiftCh1) &&
        (dp0->tBase         == dp1->tBase) &&
        (dp0->tShift        == dp1->tShift) &&
        (dp0->modeCouple0   == dp1->modeCouple0) &&
        (dp0->modeCouple1   == dp1->modeCouple1) &&
        (dp0->trigLevCh0    == dp1->trigLevCh0) &&
        (dp0->trigLevCh1    == dp1->trigLevCh1) &&
        (dp0->multiplier0   == dp1->multiplier0) &&
        (dp0->multiplier1   == dp1->multiplier1) &&
        (dp0->peakDet       == dp1->peakDet); 
    return retValue;
}
