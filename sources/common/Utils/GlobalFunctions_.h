#pragma once
#include "common/Utils/Containers/String_.h"


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

    // ����������� �������� �� ������ val �� delta. �����, ���� ��������� ������ max, ������������ ��� min
    float CircleAddFloat(float *val, float delta, float min, float max);

    // ��������� �������� �� ������ val �� delta. �����, ���� ��������� ������ min, ������������ ��� masx
    float CircleSubFloat(float *val, float delta, float min, float max);

    // ��������� � �������� �� ������ val delta. ���� ��������� �� ����������� � �������� [min; max], ����������
    void AddLimitationFloat(float *val, float delta, float min, float max);

    void IntToStrCat(char *_buffer, int _value);

    String LogArrayUint8(uint8 *array, uint size);
}
