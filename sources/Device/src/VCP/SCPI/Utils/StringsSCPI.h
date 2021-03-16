#pragma once


struct Word
{
    uint8*   address;
    int8    numSymbols;
};


int GetNumWordsInString(puchar string);                       // ���������� ���������� ���� � ������. ����������� - �������. ������ ������������� ������� 0x0d, 0x0a.
bool GetWord(puchar string, Word *word, const int numWord);   // ��� ������� ����� ����������� � �������� �������� �����.
bool WordEqualZeroString(Word *word, char* string);
bool EqualsStrings(pchar str1, pchar str2, int size);
bool EqualsZeroStrings(char *str1, char *str2);
