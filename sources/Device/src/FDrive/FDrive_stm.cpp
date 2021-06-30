// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/USBH/USBH_.h"
#include "Display/Warnings.h"
#include "FDrive/FDrive.h"
#include "FPGA/FPGA_Types.h"
#include "FPGA/Data/DataSettings.h"
#include "FPGA/Data/Storage.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <ff.h>
#include <ff_gen_drv.h>
#include <usbh_def.h>
#include <usbh_diskio_.h>
#include <cstring>


static FATFS USBDISKFatFs;
static char USBDISKPath[4];
static bool isConnected = false;

bool FDrive::needOpenFileMananger = false;
StructForWrite *FDrive::sfw = nullptr;

MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;


static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8 id);



void FDrive::Init(void)
{
    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
        USBH_Init((USBH_HandleTypeDef *)USBH::handle, USBH_UserProcess, 0);
        USBH_RegisterClass((USBH_HandleTypeDef *)USBH::handle, USBH_MSC_CLASS);
        USBH_Start((USBH_HandleTypeDef *)USBH::handle);
    }
}


void FDrive::Update(void)
{
    USBH_Process((USBH_HandleTypeDef *)USBH::handle);
}


void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;
        case HOST_USER_CLASS_ACTIVE:
            isConnected = true;
            FM::Init();
            FDrive::ChangeState();
            if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
            {
                //LOG_ERROR("Не могу примонтировать диск");
            }
            break;
        case HOST_USER_CLASS_SELECTED:
            break;
        case HOST_USER_CONNECTION:
            f_mount(NULL, (TCHAR const*)"", 0);
            break;
        case HOST_USER_DISCONNECTION:
            isConnected = false;
            FDrive::ChangeState();
            break;
        default:
            // здесь ничего
            break;
    }
}


bool FDrive::AppendStringToFile(const char*)
{
    return false;
}


void WriteToFile(FIL *file, const char *string)
{
    //    uint bytesWritten;
    f_open(file, "list.txt", FA_OPEN_EXISTING);
    //f_write(file, string, strlen(string), (void*)&bytesWritten);
    f_puts(string, file);
    f_close(file);
}


void FDrive::GetNumDirsAndFiles(const char* fullPath, int *numDirs, int *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;
    

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, fullPath, std::strlen(fullPath));
    nameDir[std::strlen(fullPath)] = '\0';

    fno.fsize = _MAX_LFN + 1;

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if (fno.fname[0] == 0)
            {
                if(alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            char *fn = fno.fname;
            if (fn[0] != '.')
            {
                if (fno.fattrib & AM_DIR)
                {
                    (*numDirs)++;
                }
                else
                {
                    (*numFiles)++;
                }
            }
        }
        f_closedir(&dir);
    }
}


String FDrive::GetNameDir(const char *fullPath, int numDir, StructForReadDir *s)
{
    std::memcpy(s->nameDir, fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numDirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                f_closedir(pDir);
                return String("");
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    f_closedir(pDir);
                    return String("");
                }
                alreadyNull = true;
            }
            char *fn = pFNO->fname;
            if (numDir == numDirs && ((pFNO->fattrib & AM_DIR) != 0))
            {
                return String(fn);
            }
            if (((pFNO->fattrib & AM_DIR) != 0) && (pFNO->fname[0] != '.'))
            {
                numDirs++;
            }
        }
    }

    return String("");
}


bool FDrive::GetNextNameDir(char *nameDirOut, StructForReadDir *s)
{
    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(pDir, pFNO) != FR_OK)
        {
            *nameDirOut = '\0';
            f_closedir(pDir);
            return false;
        }
        else if (pFNO->fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            char *fn = pFNO->fname;
            if (pFNO->fattrib & AM_DIR)
            {
                std::strcpy(nameDirOut, fn);
                return true;
            }
        }
    }
}


void FDrive::CloseCurrentDir(StructForReadDir *s)
{
    f_closedir(&s->dir);
}


bool FDrive::GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    std::memcpy(s->nameDir, fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numFiles = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            char *fn = pFNO->fname;
            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
            {
                std::strcpy(nameFileOut, fn);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                numFiles++;
            }
        }
    }
    return false;
}


bool FDrive::GetNextNameFile(char *nameFileOut, StructForReadDir *s)
{
    FILINFO *pFNO = &s->fno;
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
            char *fn = pFNO->fname;
            if ((pFNO->fattrib & AM_DIR) == 0 && pFNO->fname[0] != '.')
            {
                std::strcpy(nameFileOut, fn);
                return true;
            }
        }
    }
}


bool FDrive::OpenNewFileForWrite(const char* fullPathToFile)
{
    sfw = new StructForWrite;

    if (f_open(&sfw->fileObj, fullPathToFile, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        return false;
    }

    std::strcpy(sfw->name, fullPathToFile);

    sfw->sizeData = 0;

    return true;
}


bool FDrive::WriteToFile(uint8* data, int sizeData)
{
    while (sizeData > 0)
    {
        int dataToCopy = sizeData;

        if (sizeData + sfw->sizeData > SIZE_FLASH_TEMP_BUFFER)
        {
            dataToCopy = SIZE_FLASH_TEMP_BUFFER - sfw->sizeData;
        }

        sizeData -= dataToCopy;
        std::memcpy(sfw->tempBuffer + sfw->sizeData, data, (uint)dataToCopy);
        data += dataToCopy;
        sfw->sizeData += dataToCopy;

        if (sfw->sizeData == SIZE_FLASH_TEMP_BUFFER)
        {
            uint wr = 0;

            FRESULT result = f_write(&sfw->fileObj, sfw->tempBuffer, (uint)sfw->sizeData, &wr);

            if (result != FR_OK || (uint)sfw->sizeData != wr)
            {
                return false;
            }

            sfw->sizeData = 0;
        }
    }

    return true;
}


bool FDrive::WriteStringToFile(pchar string)
{
    String text(string);

    text.Append("\n");

    return WriteToFile((uint8 *)text.c_str(), (int)text.Size());
}


bool FDrive::CloseFile()
{
    if (sfw->sizeData != 0)
    {
        uint wr = 0;

        FRESULT result = f_write(&sfw->fileObj, sfw->tempBuffer, (uint)sfw->sizeData, &wr);

        if (result != FR_OK || (uint)sfw->sizeData != wr)
        {
            f_close(&sfw->fileObj);

            delete sfw;

            return false;
        }
    }

    f_close(&sfw->fileObj);

    FILINFO fno;
    PackedTime time = HAL_RTC::GetPackedTime();
    fno.fdate = (WORD)(((time.year + 20) * 512) | (time.month * 32) | time.day);
    fno.ftime = (WORD)((time.hours * 2048) | (time.minutes * 32) | (time.seconds / 2));
    f_utime(sfw->name, &fno);

    delete sfw;

    return true;
}


void FDrive::ChangeState()
{
    if (!isConnected)
    {
        if (Menu::GetNameOpenedPage() == NamePage::SB_FileManager)
        {
            ((Page *)Item::Opened())->ShortPressOnItem(0);
        }
    }
    else if (set.memory.flash_auto_connect)
    {
        needOpenFileMananger = true;
    }
}


bool FDrive::IsConnected()
{
    return isConnected;
}


void FDrive::SaveCurrentSignal()
{
    if (set.memory.mode_save_signal.IsBMP())
    {
        Painter::SaveScreenToFlashDrive();
    }
    else
    {
        DataReading data;

        Storage::ExtractLast(data);

        DataSettings &ds = data.Settings();

        if (!ds.IsEnabled(ChA) && !ds.IsEnabled(ChB))
        {
            return;
        }

        String fileName = CreateFileName("csv");

        OpenNewFileForWrite(fileName.c_str());

        WriteStringToFile(String("%d - top", Value::MAX).c_str());
        WriteStringToFile(String("%d - center", Value::AVE).c_str());
        WriteStringToFile(String("%d - bottom", Value::MIN).c_str());

        if (ds.IsEnabled(ChA))
        {
            WriteStringToFile("Data 1:");

            uint numBytes = ds.BytesInChannel();
            uint8 *bytes = data.Data(ChA);

            for (uint i = 0; i < numBytes; i++)
            {
                WriteStringToFile(String("%d : %d", i, bytes[i]).c_str());
            }
        }

        if (ds.IsEnabled(ChB))
        {
            WriteStringToFile("Data 2:");

            uint numBytes = ds.BytesInChannel();
            uint8 *bytes = data.Data(ChB);

            for (uint i = 0; i < numBytes; i++)
            {
                WriteStringToFile(String("%d : %d", i, bytes[i]).c_str());
            }
        }

        CloseFile();

        Warnings::ShowWarningGood("ФАЙЛ СОХРАНЕН", "FILE IS SAVED");
    }
}


String FDrive::CreateFileName(pchar extension)
{
    String result("picture.");

    result.Append(extension);

    return result;
}
