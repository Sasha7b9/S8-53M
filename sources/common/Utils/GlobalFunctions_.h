#pragma once
#include "common/Utils/String_.h"


struct Float;


namespace GF
{
    String FloatFract2String(float value, bool always_sign = false);

    // alwaysSign - ���� ���������� � true, ����� ������ ������ ����� ����, ���� ���������� � false, ������ ���� �����
    //              �������������
    // numDigits - ����� �������� ��������
    String Float2String(const Float &value, bool always_sign, int num_digits);

    // ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������.
    String Bin2String(uint8 value);

    String Bin2String16(uint16 value);

    String Hex8toString(uint8 value);

    String Int2String(int value, bool always_sign = false, int num_min_fields = 1);

    bool String2Int(char *str, int *value);

    String Voltage2String(const Float &voltage, bool always_sign);

    String Time2String(const Float &time, bool always_sign);

    String Freq2String(const Float &freq, bool always_sign = false);

    String Phase2String(float phase, bool);

    // ����������� �������� � ��������� ������.
    String Float2Db(float value, int num_digits);

    // ���������� true, ���� value ������ � �������� [min; max]
    bool IntInRange(int value, int min, int max);

    // ���������� ������������ �������� �� ���
    float MaxFloat(float val1, float val2, float val3);

    // ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min
    float CircleAddFloat(float *val, float delta, float min, float max);

    // ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx
    float CircleSubFloat(float *val, float delta, float min, float max);

    // ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������
    void AddLimitationFloat(float *val, float delta, float min, float max);

    // ���������� ������� ���������� ������ �� ������� value0 � value1
    void SwapInt(int *value0, int *value1);

    void IntToStrCat(char *_buffer, int _value);

    String LogArrayUint8(uint8 *array, uint size);

    // ����������� �������� �� ������ val �� 1. �����, ���� ��������� ��������� max, ������������ ��� � min
    template<class T> T CircleIncrease(T *val, T min, T max);

    // �������� �������� �� ������ val �� 1. �����, ���� ��������� ������ min, ������������ ��� max
    template<class T> T CircleDecrease(T *val, T min, T max);
}
