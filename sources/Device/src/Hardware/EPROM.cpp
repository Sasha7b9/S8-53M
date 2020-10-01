#include "defines.h"
#include "common/Hardware/HAL/HAL.h"
#include "common/Log.h"
#include "Hardware/EPROM.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include <cstring>





struct RecordConfig
{
    uint addrData;  // ������� � ����� ������ �������� ������. ���� addrData == MAX_UINT, �� ��� ������ ������, ���� ����� ������ ������
    int  sizeData;  // ������ � ������ ���������� ������
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_ARRAY_POINTERS_IN_ELEMENTS 1024
#define ADDR_ARRAY_POINTERS (ADDR_SECTOR_SETTINGS + 4)
#define SIZE_ARRAY_POINTERS_IN_BYTES (SIZE_ARRAY_POINTERS_IN_ELEMENTS * sizeof(RecordConfig))
static const uint ADDR_FIRST_SET = ADDR_ARRAY_POINTERS + SIZE_ARRAY_POINTERS_IN_BYTES;      // �� ����� ������ �������� ������ ���������
static const uint SIZE_MEMORY = 1024 * 1024 + 0x08000000;


// ������� ����, ��� ������ � ���� ������� ���� ��� �������������. ���� ������� ����� ������� (������, �������� ��� ��������) ����� ��� ��������, 
// ������ ��� ���� ����������� ��� ������� ���� ���
static const uint MARK_OF_FILLED = 0x1234;
static const uint MAX_UINT = 0xffffffff;

#define READ_WORD(address) (*((volatile uint*)(address)))



static const uint startDataInfo = ADDR_SECTOR_DATA_MAIN;



void EPROM::PrepareSectorForData()
{
    EraseSector(ADDR_SECTOR_DATA_MAIN);
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        HAL_EPROM::WriteWord(startDataInfo + i * 4, 0);
    }
}


bool EPROM::LoadSettings(void)
{
    /*
        1. �������� �� ������ ���������. ����������� ���, ��� � ������ ����� ������� �������� �������� MAX_UINT, ���� ��������� ��� �� �����������.
        2. �������� �� ������ ������ �������� ��������. ������������ ���, ��� � ������ ������ � ������ ����� ������� �������� �������� ��������
        MARK_OF_FILLED, � � ����� - ������ ��������� Settings (������ ����� ���� �� ���� � ��������� Settings).
        3. ���� ������ ������ - ������ �������� � ���������� � ����� �������.
            1. ����� ���������, ����� ������� ����� ����� ��������� ����������� ��������.
            2. ���� (����� + sizeof(Settings) >= ADDR_SECTORSETTINGS + (1024 * 128)), �� ��� �������� ���������� � ����� ��������� �������������
               ����������� ���������.
            3. ����� ����� ����� ��������� ����������� ��������, ������ �� �� ������ &set.display, ���������� � set.size ������ ������ ���������
            Settings � �������� Flash_SaveSettings().
    */

    HAL_EPROM::ClearFlags();

    if(TheFirstInclusion())                                         // ���� ��� ������ ���������
    {                                                               // �� ������ ��������������� �������������
        set.common.countErasedFlashSettings = 0;
        set.common.countEnables = 0;
        set.common.countErasedFlashData = 0;
        set.common.workingTimeInSecs = 0;
        PrepareSectorForData();
    }
    
    if(READ_WORD(ADDR_SECTOR_SETTINGS) == 0x12345)
    {
        EraseSector(ADDR_SECTOR_SETTINGS);
    }
    else if (READ_WORD(ADDR_SECTOR_SETTINGS) == MARK_OF_FILLED)                             // ���� ������ �������� �������� ��������
    {
        RecordConfig *record = RecordConfigForRead();
        if (record->sizeData + record->addrData >= (ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS))   // ���� ��������� ����������� ��������� �������
        {                                                                                   // �� ������� ������� (���� ���������� ������ ����������)
            --record;                                                                       // �� ������������� ����������� ������������ �����������
        }
        std::memcpy(&set, (const void *)(record->addrData - 4), static_cast<uint>(record->sizeData));               // ��������� ��
        EraseSector(ADDR_SECTOR_SETTINGS);                                                  // ������� ������ ��������
        EPROM::SaveSettings(true);                                                           // � ��������� ��������� � ����� �������
    }
    else
    {
        uint addressPrev = 0;
        uint address = ADDR_SECTOR_SETTINGS;
        while (READ_WORD(address) != MAX_UINT)  // ���� �� ����� ������ ���� ��������, �������� �� (-1) (���� ������������� ������)
        {
            addressPrev = address;              // ��������� ���� �����
            address += READ_WORD(address);      // � ��������� � ���������� ������������ ��������, ����������� �� ����� ������ (������ �������
        }                                       // ��������� Settings - � ������. ��� ��������� �������� ��������������� � ������, ���� ���������
                                                // �� ������
        
        if (addressPrev != 0)                   // ���� �� ����� ������ ���-�� ��������
        {
            std::memcpy(&set, (const void *)addressPrev, (uint)READ_WORD(addressPrev));    // �������� ����������� ���������
            return true;
        }
    }
    set.common.countEnables++;
    return false;
}


void EPROM::WriteAddressDataInRecord(RecordConfig *record)
{
    uint address = (record == FirstRecord()) ? ADDR_FIRST_SET : (record - 1)->addrData + (record - 1)->sizeData;

    HAL_EPROM::WriteWord((uint)(&record->addrData), address);
}


void EPROM::SaveSettings(bool verifyLoadede)
{
    if (!verifyLoadede && !SETTINGS_IS_LOADED)
    {
        return;
    }

    HAL_EPROM::ClearFlags();

    set.size = sizeof(set);

    uint address = ADDR_SECTOR_SETTINGS;                                        // ������� ������ ��������� ����

    while (READ_WORD(address) != MAX_UINT)
    {
        address += READ_WORD(address);
    }
                                                                                // � ���� ����� address ��������� �� ������ ������������ ����

    if (address + sizeof(set) >= (ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS)) // ���� ������� �����������, �� ��� ������ ������ ������ �� �������
    {                                                                           // �������
        EraseSector(ADDR_SECTOR_SETTINGS);                                      // � ���� ������ ������� ������ ��������
        address = ADDR_SECTOR_SETTINGS;                                         // � ��������� ��������� ����� ����� � ������ �������
    }

    HAL_EPROM::WriteBufferBytes(address, (uint8 *)&set, sizeof(set));                      // � �������� ��������� ���������
}


bool EPROM::TheFirstInclusion()
{
    return READ_WORD(ADDR_SECTOR_SETTINGS) == MAX_UINT;
}


RecordConfig* EPROM::RecordConfigForRead()
{
    if (!TheFirstInclusion())
    {
        RecordConfig *record = FirstEmptyRecord();
        return --record;
    }

    return 0;
}


RecordConfig *EPROM::FirstRecord()
{
    return (RecordConfig*)ADDR_ARRAY_POINTERS;
}


bool EPROM::RecordExist()
{
    return READ_WORD(ADDR_ARRAY_POINTERS) != MAX_UINT;
}


RecordConfig *EPROM::FirstEmptyRecord()
{
    RecordConfig *record = FirstRecord();
    int numRecord = 0;

    while (record->addrData != MAX_UINT)
    {
        record++;
        if ((++numRecord) == SIZE_ARRAY_POINTERS_IN_ELEMENTS)
        {
            return 0;
        }
    }

    return record;
}


uint EPROM::CalculatFreeMemory()
{
    if (!RecordExist())
    {
        return SIZE_MEMORY - ADDR_FIRST_SET;
    }

    RecordConfig *firstEmptyRecord = FirstEmptyRecord();

    if (firstEmptyRecord == 0)  // ���� ��� ������ ������
    {
        return 0;
    }

    return SIZE_MEMORY - (firstEmptyRecord - 1)->addrData - (firstEmptyRecord - 1)->sizeData - 4;
}


uint EPROM::FindAddressNextDataInfo()
{
    uint addressNextInfo = startDataInfo + MAX_NUM_SAVED_WAVES * 4;

    while (*((uint*)addressNextInfo) != 0xffffffff)
    {
        addressNextInfo = *((uint*)addressNextInfo) + MAX_NUM_SAVED_WAVES * 4;
    }

    return addressNextInfo;
}


uint EPROM::FindActualDataInfo()
{
    return FindAddressNextDataInfo() - MAX_NUM_SAVED_WAVES * 4;
}


void EPROM::GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    uint address = FindActualDataInfo();

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = READ_WORD(address + i * 4) != 0;
    }
}


bool EPROM::ExistData(int num)
{
    uint address = FindActualDataInfo();
    return READ_WORD(address + num * 4) != 0;
}


void EPROM::DeleteData(int num)
{
    uint address = FindActualDataInfo();
    HAL_EPROM::WriteWord(address + num * 4, 0);
}


void EPROM::EraseData()
{
    HAL_EPROM::EraseSector(8);
    PrepareSectorForData();
}


int EPROM::CalculateSizeData(DataSettings *ds)
{
    int size = sizeof(DataSettings);
    if (ds->enableCh0 == 1)
    {
        size += ds->length1channel;
    }
    if (ds->enableCh1 == 1)
    {
        size += ds->length1channel;
    }
    return size;
}


uint EPROM::FreeMemory()
{
    return ADDR_SECTOR_DATA_MAIN + 128 * 1024 - FindAddressNextDataInfo() - 1 - 4 * MAX_NUM_SAVED_WAVES - 3000;
}


void EPROM::CompactMemory()
{
    Display::ClearFromWarnings();
    Display::ShowWarningGood(MovingData);
    Display::Update();
    uint dataInfoRel = FindActualDataInfo() - ADDR_SECTOR_DATA_MAIN;

    EraseSector(ADDR_SECTOR_DATA_HELP);
    HAL_EPROM::WriteBufferBytes(ADDR_SECTOR_DATA_HELP, (uint8*)ADDR_SECTOR_DATA_MAIN, 1024 * 128);
    PrepareSectorForData();

    uint addressDataInfo = ADDR_SECTOR_DATA_HELP + dataInfoRel;

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        uint addrDataOld = READ_WORD(addressDataInfo + i * 4);
        if (addrDataOld != 0)
        {
            uint addrDataNew = addrDataOld + 1024 * 128;
            DataSettings *ds = (DataSettings*)addrDataNew;
            addrDataNew += sizeof(DataSettings);
            uint8 *data0 = 0;
            uint8 *data1 = 0;
            if (ds->enableCh0 == 1)
            {
                data0 = (uint8*)addrDataNew;
                addrDataNew += ds->length1channel;
            }
            if (ds->enableCh1 == 1)
            {
                data1 = (uint8*)addrDataNew;
            }
            EPROM::SaveData(i, ds, data0, data1);
        }
    }
    Display::ClearFromWarnings();
}


void EPROM::SaveData(int num, DataSettings *ds, uint8 *data0, uint8 *data1)
{
    /*
        1. ����� ���������� ���������� ������.
        2. ���� �� ������� ��� ������ ������ � ������� ���������� - ��������� ������.
        3. ������� ��������� ����������� ������ ����������.
        4. � ����� ��� ����� �����, � ������� ����� ��������� ����. ������ ����������.
        5. ���������� �� ��� ������.
        6. �� ������� ���������� ���������� ������ ����������.
    */

    /*
        �������� ���������� ������.
        ADDR_SECTOR_DATA_HELP ������������ ��� ���������� �������� ������, ����� ��������� �������� ADDR_SECTOR_DATA_MAIN � ������ ������ ��������� ������

        ������ ����������� � ������ ��������� �������.
    
        ���������� � ADDR_SECTOR_DATA_MAIN.

        uint[MAX_NUM_SAVED_WAVES] - ������, �� ������� �������� ��������������� �������. ���� 0 - ������ ����.
        uint - ����� ������ ��������� ������ ������ (���������� ������� �������). � �� ����� ������� ����� ������� �������.
    */

    if (ExistData(num))
    {
        DeleteData(num);
    }

    int size = CalculateSizeData(ds);

// 2
    if (FreeMemory() < static_cast<uint>(size))
    {
        CompactMemory();
    }

// 3
    uint addrDataInfo = FindActualDataInfo();          // ������� ������ ������������ ��������������� �������

// 4
    uint addrForWrite = addrDataInfo + MAX_NUM_SAVED_WAVES * 4;             // ��� - �����, �� �������� ����� ��������� ����� ���������� ��������������� �������
    uint valueForWrite = addrForWrite + 4 + size;                           // ��� - ����� ���������� ��������������� �������
    HAL_EPROM::WriteWord(addrForWrite, valueForWrite);

// 5
    uint address = addrDataInfo + MAX_NUM_SAVED_WAVES * 4 + 4;              // �����, �� �������� ����� �������� ������ � �����������
    uint addressNewData = address;
    HAL_EPROM::WriteBufferBytes(address, (uint8*)ds, sizeof(DataSettings));            // ��������� ��������� �������
    address += sizeof(DataSettings);
    
    if (ds->enableCh0 == 1)
    {
        HAL_EPROM::WriteBufferBytes(address, (uint8*)data0, (int)ds->length1channel);       // ��������� ������ �����
        address += ds->length1channel;
    }

    if (ds->enableCh1 == 1)
    {
        HAL_EPROM::WriteBufferBytes(address, (uint8*)data1, (int)ds->length1channel);       // ��������� ������ �����
        address += ds->length1channel;
    }

// 6
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        uint addressForWrite = address + i * 4;
        if (i == num)
        {
            HAL_EPROM::WriteWord(addressForWrite, addressNewData);
        }
        else
        {
            HAL_EPROM::WriteWord(addressForWrite, READ_WORD(addrDataInfo + i * 4));
        }
    }
}


bool EPROM::GetData(int num, DataSettings **ds, uint8 **data0, uint8 **data1)
{
    uint addrDataInfo = FindActualDataInfo();
    if (READ_WORD(addrDataInfo + 4 * num) == 0)
    {
        *ds = 0;
        *data0 = 0;
        *data1 = 0;
        return false;
    }

    uint addrDS = READ_WORD(addrDataInfo + 4 * num);

    uint addrData0 = 0;
    uint addrData1 = 0;

    *ds = (DataSettings*)addrDS;
    
    if ((*ds)->enableCh0 == 1)
    {
        addrData0 = addrDS + sizeof(DataSettings);
    }

    if ((*ds)->enableCh1 == 1)
    {
        if (addrData0 != 0)
        {
            addrData1 = addrData0 + (*ds)->length1channel;
        }
        else
        {
            addrData1 = addrDS + sizeof(DataSettings);
        }
    }

    *data0 = (uint8*)addrData0;
    *data1 = (uint8*)addrData1;
    
    return true;
}


void EPROM::EraseSector(uint startAddress)
{
    HAL_EPROM::EraseSector(HAL_EPROM::GetSector(startAddress));
}
