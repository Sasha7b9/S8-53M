#pragma once
#include "FPGA/FPGA.h"


class Storage
{
public:
    
    // ���������� ��������� ��� ��������� ������.
#ifdef DEBUG
    static const int SIZE_POOL = (60 * 1024);
#else
    static const int SIZE_POOL = (61 * 1024);
#endif

    // �������� ���� ����������� ���������
    static void Clear();
    // �������� ��������� ������. ��� ���� ����������� ��������� ������� ��������� �������.
    static void AddData(uint8 *data_a, uint8 *data_b, DataSettings dss);
    // ���������� ����� ����������� ���������, ������� � ����������, � ������ �� �����������, ��� � ����������.
    static int NumElementsWithSameSettings();
    // ���������� ����� ����������� ���������, ������� � ����������, � �������� ����������� �������
    static int NumElementsWithCurrentSettings();
    // ���������� ���������� ����������� ���������
    static int NumElementsInStorage();
    // �������� ��������� �� ������
    static bool GetDataFromEnd(int fromEnd, DataSettings **ds, uint8 **data0, uint8 **data1);

    static uint8* GetData(Channel::E ch, int fromEnd);
    // �������� ����������� ������ �� ���������� ����������.
    static uint8* GetAverageData(Channel::E ch);
    // ������� ����� ��������� ��������� � ������.
    static int AllDatas();
    // �������� �������������� ����� ������� 0 - �����, 1 - ������.
    static uint8* GetLimitation(Channel::E ch, int direction);                                      

    static int NumberAvailableEntries();

    static uint8 *dataA;            // ��������� �� ������ ������� ������, ������� ���� �������� �� ������
    static uint8 *dataB;            // ��������� �� ������ ������� ������, ������� ���� �������� �� ������
    static DataSettings *set;       // ��������� �� ��������� �������� ��������

    static uint8 *dataIntA;         // ����� ������ �� ����, ������� ������ ���������� �� �����
    static uint8 *dataIntB;
    static DataSettings *dsInt;
    
    static uint8 *dataLastA;
    static uint8 *dataLastB;
    static DataSettings *dsLast;

private:

    static void CalculateSums();
    // ���������� ���������� ��������� ������ � ������
    static int MemoryFree();
    // ���������, ������� ������ ���������, ����� ��������� ��������� � ����������� dp
    static int SizeElem(const DataSettings *dp);
    // ������� ������ (����� ������) ���������
    static void RemoveFirstElement();
    // ��������� ������
    static void PushData(DataSettings *dp, uint16 * data0, uint16 * data1);
    // ���������� ��������� �� ���������, ��������� �� elem
    static DataSettings* NextElem(DataSettings *elem);
    // ���������� ��������� �� ������, ��������� �� indexFromEnd o� ���������� ������������
    static DataSettings* FromEnd(int indexFromEnd);
    // ���������� true, ���� ��������� ��������� � ��������� elemFromEnd0 � elemFromEnd1 ���������, � false � ���� ������.
    static bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1);
    // ��������� true, ���� ��������� � ����� ���������� ���������
    static bool SettingsIsEquals(const DataSettings *dp0, const DataSettings *dp1);
    // ������� �������� ���, ���� � ����
    static void ClearLimitsAndSums();

    static void CalculateLimits(uint16 * data0, uint16 * data1, const DataSettings *dss);

    static DataSettings* GetSettingsDataFromEnd(int fromEnd);
    // �������� ������ ������ chan ��, ������������ ds, � ���� �� ���� ����� ������� dataImportRel. ������������ �������� false ��������, ��� ������ ����� ��������.
    static bool CopyData(DataSettings *ds, Channel::E ch, uint8 datatImportRel[NumChannels][FPGA_MAX_POINTS]);

    static void PrintElement(DataSettings *dp);
    
    static void CalculateAroundAverage(uint16 *data0, uint16 *data1, const DataSettings *dss);
    // ����� �������� ������.
    static uint8 pool[SIZE_POOL];
    // ����� ������ ������ ��� ��������
    static uint8* beginPool;
    // ����� ���������� ����� ������ ��� ��������
    static uint8* endPool;
    // ����� �������� ����� ��������� ����� �������
    static uint  sum[NumChannels][FPGA_MAX_POINTS];
    // ������������ �������� �������
    static uint8 limitUp[NumChannels][FPGA_MAX_POINTS];
    // ����������� �������� �������
    static uint8 limitDown[NumChannels][FPGA_MAX_POINTS];
    // ��������� �� ������ ����������� ������
    static DataSettings* firstElem;
    // ��������� �� ��������� ����������� ������
    static DataSettings* lastElem;
    // ����� ������ ���������
    static int allData;
    // � ���� �������� �������� ����������� ��������, ������������ �� ���������������� ���������.
    static float aveData0[FPGA_MAX_POINTS];

    static float aveData1[FPGA_MAX_POINTS];
    // ���� true, �� ����� ����� ����������, � ����� ��������� ������ ��������
    static bool newSumCalculated[NumChannels];

};
