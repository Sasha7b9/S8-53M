#pragma once


#define MALLOC_EXTRAMEM(NameStruct, name)   RAM::extra = std::malloc(sizeof(NameStruct));    \
                                            NameStruct *name = (NameStruct*)RAM::extra
#define ACCESS_EXTRAMEM(NameStruct, name)   NameStruct *name = (NameStruct*)RAM::extra //-V2546 //-V1003
#define FREE_EXTRAMEM()                     std::free(RAM::extra)


struct RAM
{
    static void *extra; // ��� ����������� ���������. ������������ ��� ��������� ������ ����������, ������� �� ����� �� ����� ��������� ���������,
                        // �� ����� ������ ��� � ����� �������. ����� �������������� � ������� ������ malloc() ���������� ����������� ����������
                        // ������, ������� ����� ������������� ������ free()

};
