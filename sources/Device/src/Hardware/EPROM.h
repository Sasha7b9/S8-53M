#pragma once


#define MAX_NUM_SAVED_WAVES 23  // ѕока ограничено количеством квадратиков, которые можно вывести в одну линию внизу сетки

struct RecordConfig;

class EPROM
{
friend class OTP;
    
public:
    static bool LoadSettings();
    static void SaveSettings(bool verifyLoaded = false);
    // ≈сли даннные есть, соответствующий элемент массива равен true/.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
    static bool ExistData(int num);
    static void SaveData(int num, DataSettings *ds, uint8 *data0, uint8 *data1);
    static bool GetData(int num, DataSettings **ds, uint8 **data0, uint8 **data1);
    static void DeleteData(int num);
    // —тирает сектора с данными
    static void EraseData();

private:
    
    static void EraseSector(uint startAddress);
    static void PrepareSectorForData();
    static bool TheFirstInclusion();
    static RecordConfig* RecordConfigForRead();
    static void WriteAddressDataInRecord(RecordConfig *record);
    static RecordConfig *FirstRecord();
    static RecordConfig *FirstEmptyRecord();
    static uint CalculatFreeMemory();
    static void CompactMemory();
    static int CalculateSizeData(DataSettings *ds);
    static uint FindActualDataInfo();
    static uint FindAddressNextDataInfo();
    static uint FreeMemory();
    static bool RecordExist();
};
