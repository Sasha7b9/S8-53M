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

    // ����������� �������� � ��������� ������.
    String Float2Db(float value, int num_digits);

    void IntToStrCat(char *_buffer, int _value);

    String LogArrayUint8(uint8 *array, uint size);
}
