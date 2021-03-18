#define _STL_COMPILER_PREPROCESSOR 0
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Hardware/USBD/usbd_desc.h"
#include "Hardware/USBD/usbd_cdc_interface.h"
#include <usbd_def.h>
#include <stm32f4xx_hal.h>


static USBD_HandleTypeDef handleUSBD;
void *HAL_USBD::handle = &handleUSBD;


void HAL_USBD::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, 0);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
//    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
//    USBD_Start(&handleUSBD);
}


bool HAL_USBD::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}


void HAL_USBD::Transmit(uint8 *buffer, int size)
{
    USBD_CDC_SetTxBuffer(&handleUSBD, buffer, (uint16)size);
    USBD_CDC_TransmitPacket(&handleUSBD);
}


void HAL_USBD::Flush(uint8 *buffer, int sizeBuffer)
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    while (pCDC->TxState == 1)
    {
    };
    USBD_CDC_SetTxBuffer(&handleUSBD, buffer, (uint16)sizeBuffer);
    USBD_CDC_TransmitPacket(&handleUSBD);
    while (pCDC->TxState == 1)
    {
    };
}


void HAL_USBD::Wait()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;

    while (pCDC->TxState == 1)
    {
    }
}
