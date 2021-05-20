// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "main.h"


void FDrive_Init();
bool FDrive_Update();
bool FDrive_FileExist(pchar fileName);
int FDrive_OpenFileForRead(pchar fileName);
// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
uint FDrive_ReadFromFile(int numBytes, uint8 *buffer);
void FDrive_CloseOpenedFile();
