#pragma once


struct Word
{
    uint8*   address;
    int8    numSymbols;
};


int GetNumWordsInString(pUCHAR string);                       // ���������� ���������� ���� � ������. ����������� - �������. ������ ������������� ������� 0x0d, 0x0a.
bool GetWord(pUCHAR string, Word *word, const int numWord);   // ��� ������� ����� ����������� � �������� �������� �����.
bool WordEqualZeroString(Word *word, char* string);
bool EqualsStrings(char *str1, char *str2, int size);
bool EqualsZeroStrings(char *str1, char *str2);
