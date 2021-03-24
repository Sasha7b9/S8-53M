#pragma once
#include "common/Utils/Containers/String_.h"


struct Float;


namespace GF
{
    // ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������.
    String Bin2String(uint8 value);

    String Bin2String16(uint16 value);

    String Hex8toString(uint8 value);

    // ����������� �������� � ��������� ������.
    String Float2Db(float value, int num_digits);

    String LogArrayUint8(uint8 *array, uint size);

    int NumDigitsInIntPart(float value);
}
