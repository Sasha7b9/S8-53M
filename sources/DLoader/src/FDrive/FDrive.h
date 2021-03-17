#pragma once
#include "main.h"


void FDrive_Init();
bool FDrive_Update();
bool FDrive_FileExist(pchar fileName);
int FDrive_OpenFileForRead(pchar fileName);
// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
int FDrive_ReadFromFile(int numBytes, uint8 *buffer);
void FDrive_CloseOpenedFile();
