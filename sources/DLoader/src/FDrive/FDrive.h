// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "main.h"


typedef enum
{
    APPLICATION_IDLE = 0,
    APPLICATION_START,
    APPLICATION_RUNNING,
}MSC_ApplicationTypeDef;


extern MSC_ApplicationTypeDef Appli_state;


void FDrive_Init();
bool FDrive_Update();
bool FDrive_FileExist(pchar fileName);
int FDrive_OpenFileForRead(pchar fileName);
// Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
uint FDrive_ReadFromFile(int numBytes, uint8 *buffer);
void FDrive_CloseOpenedFile();
