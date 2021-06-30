// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h" 
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/USBH/USBH_.h"
#include <usbh_def.h>
#include <ff_gen_drv.h>
#include <usbh_diskio_.h>
#include <usbh_core.h>
#include <usbh_msc.h>
#include "ffconf.h"
#include "FDrive/FDrive.h"
#include "main.h"
#include <ctype.h>
#include <cstring>


MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;


struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    char lfn[(_MAX_LFN + 1)];
    FILINFO fno;
    DIR dir;
};



static bool GetNameFile(pchar fullPath, int numFile, char *nameFileOut, StructForReadDir *s);
static bool GetNextNameFile(char *nameFileOut, StructForReadDir *s);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8 id);



void FDrive_Init()
{
    MainStruct::ms->drive.state = StateDisk::Idle;
    MainStruct::ms->drive.connected = 0;
    MainStruct::ms->drive.active = 0;

    if (FATFS_LinkDriver(&USBH_Driver, MainStruct::ms->drive.USBDISKPath) == FR_OK)
    {
        USBH_Init((USBH_HandleTypeDef *)USBH::handle, USBH_UserProcess, 0);
        USBH_RegisterClass((USBH_HandleTypeDef *)USBH::handle, USBH_MSC_CLASS);
        USBH_Start((USBH_HandleTypeDef *)USBH::handle);
    }
}


void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            MainStruct::ms->drive.active++;
            MainStruct::ms->drive.state = StateDisk::Start;
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            MainStruct::ms->drive.connected++;
            MainStruct::ms->state = State::Mount;
            f_mount(NULL, (TCHAR const*)"", 0);
            break;

        case HOST_USER_DISCONNECTION:
            break;

        default:
            break;
    }
}


bool FDrive_Update()
{
//    USBH_Process(reinterpret_cast<USBH_HandleTypeDef *>(USBH::handle));
//    if (MainStruct::ms->drive.state == StateDisk::Start)
//    {
//        if (f_mount(&(MainStruct::ms->drive.USBDISKFatFS), (TCHAR const*)MainStruct::ms->drive.USBDISKPath, 0) == FR_OK)
//        {
//            return true;
//        }
//        else
//        {
//            MainStruct::ms->state = State::WrongFlash;
//            return false;
//        }
//    }
    return false;
}


void ToLower(char *str)
{
    while (*str)
    {
        *str = (char)(tolower(*str));
        str++;
    }
}


bool FDrive_FileExist(pchar fileName)
{
    char nameFile[255];
    char file[255];
    std::strcpy(file, fileName);
    ToLower(file);
    StructForReadDir strd;

    if (GetNameFile("\\", 0, nameFile, &strd))
    {
        ToLower(nameFile);
        if (std::strcmp(file, nameFile) == 0)
        {
            return true;
        }
        while (GetNextNameFile(nameFile, &strd))
        {
            ToLower(nameFile);
            if (std::strcmp(file, nameFile) == 0)
            {
                return true;
            }
        }
    }

    return false;
}


static bool GetNameFile(pchar, int, char *, StructForReadDir *)
{
//    memcpy(s->nameDir, fullPath, strlen(fullPath));
//    s->nameDir[strlen(fullPath)] = '\0';
//
//    s->fno.lfname = s->lfn;
//    s->fno.lfsize = sizeof(s->lfn);
//
//    DIR *pDir = &s->dir;
//    FILINFO *pFNO = &s->fno;
//    if (f_opendir(pDir, s->nameDir) == FR_OK)
//    {
//        int numFiles = 0;
//        bool alreadyNull = false;
//        while (true)
//        {
//            if (f_readdir(pDir, pFNO) != FR_OK)
//            {
//                *nameFileOut = '\0';
//                f_closedir(pDir);
//                return false;
//            }
//            if (pFNO->fname[0] == 0)
//            {
//                if (alreadyNull)
//                {
//                    *nameFileOut = '\0';
//                    f_closedir(pDir);
//                    return false;
//                }
//                alreadyNull = true;
//            }
//            char *fn = *(pFNO->lfname) ? pFNO->lfname : pFNO->fname;
//            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
//            {
//                strcpy(nameFileOut, fn);
//                return true;
//            }
//            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
//            {
//                numFiles++;
//            }
//        }
//    }
    return false;
}


static bool GetNextNameFile(char *nameFileOut, StructForReadDir *s)
{
//    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(&s->dir, &s->fno) != FR_OK)
        {
            *nameFileOut = '\0';
            f_closedir(&s->dir);
            return false;
        }
        if (s->fno.fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameFileOut = '\0';
                f_closedir(&s->dir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
//            char *fn = *(pFNO->lfname) ? pFNO->lfname : pFNO->fname;
//            if ((pFNO->fattrib & AM_DIR) == 0 && pFNO->fname[0] != '.')
//            {
//                strcpy(nameFileOut, fn);
//                return true;
//            }
        }
    }
}


int FDrive_OpenFileForRead(pchar fileName)
{
    if (f_open(&MainStruct::ms->drive.file, fileName, FA_READ) == FR_OK)
    {
//        return (int)ms->drive.file.fsize;
    }
    return -1;
}


uint FDrive_ReadFromFile(int numBytes, uint8 *buffer)
{
    uint readed = 0;
    if (f_read(&MainStruct::ms->drive.file, buffer, (uint)(numBytes), &readed) == FR_OK)
    {
        return readed;
    }
    return (uint)-1;
}


void FDrive_CloseOpenedFile()
{
    f_close(&MainStruct::ms->drive.file);
}
