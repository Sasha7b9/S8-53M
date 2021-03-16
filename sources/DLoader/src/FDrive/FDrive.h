#pragma once
#include "main.h"


void FDrive_Init();
bool FDrive_Update();
bool FDrive_FileExist(const char *fileName);
int FDrive_OpenFileForRead(const char *fileName);
// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
int FDrive_ReadFromFile(int numBytes, uint8 *buffer);
void FDrive_CloseOpenedFile();
