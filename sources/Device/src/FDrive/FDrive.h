// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Utils/Containers/String_.h"


#ifdef _MS_VS
#undef _WIN32
#endif

#include <ff.h>

#ifdef _MS_VS
#define _WIN32
#endif

#define SIZE_FLASH_TEMP_BUFFER 512
typedef struct
{
    uint8 tempBuffer[SIZE_FLASH_TEMP_BUFFER];
    int sizeData;
    FIL fileObj;
    char name[255];
} StructForWrite;

typedef struct
{
    char nameDir[_MAX_LFN + 1];
    char lfn[(_MAX_LFN + 1)];
    FILINFO fno;
    DIR dir;
} StructForReadDir;

class FDrive
{
public:
    static void Init();

    static void Update();

    static void GetNumDirsAndFiles(const char* fullPath, int *numDirs, int *numFiles);

    static String GetNameDir(const char* fuulPath, int numDir, StructForReadDir *sfrd);

    static bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);

    static void CloseCurrentDir(StructForReadDir *sfrd);

    static bool GetNameFile(pchar fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);

    static bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);
    // Функция создаст файл для записи. Если такой файл уже существует, сотрёт его, заменит новым нулевой длины и
    // откроет его
    static bool OpenNewFileForWrite(const char* fullPathToFile, StructForWrite *structForWrite);

    static bool WriteToFile(uint8* data, int sizeData, StructForWrite *structForWrite);

    static bool CloseFile(StructForWrite *structForWrite);

    static bool AppendStringToFile(const char* string);

    static void ChangeState();

    static bool isConnected;
    static bool needOpenFileMananger;    // Если 1, то нужно открыть файловый менеджер (сработало автоподключение)
};
