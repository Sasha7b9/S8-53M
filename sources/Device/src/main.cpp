// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/USBH/USBH_.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>
#include <usbh_def.h>
#include "usbh_diskio_.h"


FATFS USBDISKFatFs;
FIL MyFile;
typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_START,    
  APPLICATION_RUNNING,
}MSC_ApplicationTypeDef;

MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;


char USBDISKPath[4];


static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void MSC_Application(void);


int main()
{
    /*
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();
   
    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase(); 
    */
    
    setNRST.Load();

    Device::Init();

      /*##-1- Link the USB Host disk I/O driver ##################################*/
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
        /*##-2- Init Host Library ################################################*/
        USBH_Init((USBH_HandleTypeDef *)USBH::handle, USBH_UserProcess, 0);
        
        /*##-3- Add Supported Class ##############################################*/
        USBH_RegisterClass((USBH_HandleTypeDef *)USBH::handle, USBH_MSC_CLASS);
        
        /*##-4- Start Host Process ###############################################*/
        USBH_Start((USBH_HandleTypeDef *)USBH::handle);
        
        /*##-5- Run Application (Blocking mode) ##################################*/
        while (1)
        {
        /* USB Host Background task */
        USBH_Process((USBH_HandleTypeDef *)USBH::handle);
        
        /* Mass Storage Application State Machine */
        switch(Appli_state)
        {
        case APPLICATION_START:
            MSC_Application();
            Appli_state = APPLICATION_IDLE;
            break;
            
        case APPLICATION_IDLE:
        default:
            break;      
        }
        }
    }  
  
//    while(1)
//    {
//        Device::Update();
//
//        set.Save();
//
//        setNRST.Save();
//    }
}


static void MSC_Application(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */
  
  /* Register the file system object to the FatFs module */
  if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    ERROR_HANDLER();
  }
  else
  {
      /* Create and Open a new text file object with write access */
      if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
      {
        /* 'STM32.TXT' file Open for write Error */
        ERROR_HANDLER();
      }
      else
      {
        /* Write data to the text file */
        res = f_write(&MyFile, wtext, sizeof(wtext), &byteswritten);
        
        if((byteswritten == 0) || (res != FR_OK))
        {
          /* 'STM32.TXT' file Write or EOF Error */
          ERROR_HANDLER();
        }
        else
        {
          /* Close the open text file */
          f_close(&MyFile);
          
        /* Open the text file object with read access */
        if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
        {
          /* 'STM32.TXT' file Open for read Error */
          ERROR_HANDLER();
        }
        else
        {
          /* Read data from the text file */
          res = f_read(&MyFile, rtext, sizeof(rtext), &bytesread);
          
          if((bytesread == 0) || (res != FR_OK))
          {
            /* 'STM32.TXT' file Read or EOF Error */
            ERROR_HANDLER();
          }
          else
          {
            /* Close the open text file */
            f_close(&MyFile);
            
            /* Compare read data with the expected data */
            if((bytesread != byteswritten))
            {                
              /* Read data is different from the expected data */
              ERROR_HANDLER();
            }
            else
            {
            }
          }
        }
      }
    }
  }
  
  /* Unlink the USB disk I/O driver */
  FATFS_UnLinkDriver(USBDISKPath);
}


static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch(id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_IDLE;
    f_mount(NULL, (TCHAR const*)"", 0);      
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_START;
    break;
    
  default:
    break;
  }
}

