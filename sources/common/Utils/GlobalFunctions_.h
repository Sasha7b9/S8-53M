#pragma once


namespace GF
{
    char *FloatFract2String(float value, bool always_sign, char out_buffer[20]);

    // alwaysSign - ���� ���������� � true, ����� ������ ������ ����� ����, ���� ���������� � false, ������ ���� �����
    //              �������������
    // numDigits - ����� �������� ��������
    char *Float2String(float value, bool always_sign, int num_digits, char out_buffer[20]);

    // ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������.
    char *Bin2String(uint8 value, char out_buffer[9]);

    char *Bin2String16(uint16 value, char buffer[19]);

    char *Hex8toString(uint8 value, char buffer[3]);

    char *Int2String(int value, bool always_sign, int num_min_fields, char buffer[20]);

    bool String2Int(char *str, int *value);

    char *Voltage2String(float voltage, bool always_sign, char buffer[20]);

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

    char *PrintArrayUint8(uint8 *array, int size, char *out_buffer);
}
