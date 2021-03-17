#pragma once
#include "common/Utils/String_.h"


namespace GF
{
    String FloatFract2String(float value, bool always_sign = false);

    // alwaysSign - ���� ���������� � true, ����� ������ ������ ����� ����, ���� ���������� � false, ������ ���� �����
    //              �������������
    // numDigits - ����� �������� ��������
    String Float2String(float value, bool always_sign, int num_digits);

    // ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������.
    String Bin2String(uint8 value);

    String Bin2String16(uint16 value);

    String Hex8toString(uint8 value);

    String Int2String(int value, bool always_sign = false, int num_min_fields = 1);

    bool String2Int(char *str, int *value);

    String Voltage2String(float voltage, bool always_sign);

    char *Time2String(float time, bool always_sign, char buffer[20]);

    char *Freq2String(float freq, bool, char buffer_out[20]);

    char *Phase2String(float phase, bool, char bufferOut[20]);

    // ����������� �������� � ��������� ������.
    char *Float2Db(float value, int num_digits, char buffer_out[20]);

    // ���������� true, ���� value ������ � �������� [min; max]
    bool IntInRange(int value, int min, int max);

    // ���������� ������������ �������� �� ���
    float MaxFloat(float val1, float val2, float val3);

    // ����������� �������� �� ������ val �� 1. �����, ���� ��������� ��������� max, ������������ ��� � min
    template<class T>
    T CircleIncrease(T *val, T min, T max);

    // �������� �������� �� ������ val �� 1. �����, ���� ��������� ������ min, ������������ ��� max
    template<class T>
    T CircleDecrease(T *val, T min, T max);

    // ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min
    float CircleAddFloat(float *val, float delta, float min, float max);

    // ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx
    float CircleSubFloat(float *val, float delta, float min, float max);

    // ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������
    void AddLimitationFloat(float *val, float delta, float min, float max);

    // ���������� ������� ���������� ������ �� ������� value0 � value1
    void SwapInt(int *value0, int *value1);

    // ��������� ���������� value0 � value1 � ������� �����������
    void SortInt(int *value0, int *value1);

    void IntToStrCat(char *_buffer, int _value);

    String LogArrayUint8(uint8 *array, int size);
}
