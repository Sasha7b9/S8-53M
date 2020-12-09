#include "defines.h"
#include "main.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "common/Log_c.h"
#include "common/Hardware/Timer_c.h"
#include "common/Hardware/HAL/HAL_c.h"



static USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

#define APP_RX_DATA_SIZE  32
static uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */


static int8_t CDC_Itf_Init     ();
static int8_t CDC_Itf_DeInit   ();
static int8_t CDC_Itf_Control  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Itf_Receive  (uint8_t* pbuf, uint32_t *Len);


USBD_CDC_ItfTypeDef USBD_CDC_fops = 
{
  CDC_Itf_Init,
  CDC_Itf_DeInit,
  CDC_Itf_Control,
  CDC_Itf_Receive
};



static void SetAttributeConnected()
{
    VCP::cableIsConnected = true;
    VCP::connectToHost = false;
    Timer::Disable(TypeTimer::Temp);
}



static int8_t CDC_Itf_Init(void)
{
    USBD_CDC_SetRxBuffer(reinterpret_cast<USBD_HandleTypeDef *>(HAL_USBD::handle), UserRxBuffer);
    Timer::Enable(TypeTimer::Temp, 100, SetAttributeConnected);    // GOVNOCODE Задержка введена для того, чтобы не было ложных срабатываний в 
    return (USBD_OK);                                   // usbd_conf.c:HAL_PCD_SetupStageCallback при определении подключения хоста
}



static int8_t CDC_Itf_DeInit(void)
{
    VCP::cableIsConnected = false;
    VCP::connectToHost = false;

    return (USBD_OK);
}



static int8_t CDC_Itf_Control (uint8_t cmd, uint8_t* pbuf, uint16_t)
{ 
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_SET_LINE_CODING:
        LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\ //-V2533 //-V2563
                                (pbuf[2] << 16) | (pbuf[3] << 24)); //-V2563
        LineCoding.format     = pbuf[4]; //-V2563
        LineCoding.paritytype = pbuf[5]; //-V2563
        LineCoding.datatype   = pbuf[6]; //-V2563
        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8_t)(LineCoding.bitrate); //-V2533 //-V2563
        pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8); //-V2533 //-V2563
        pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16); //-V2533 //-V2563
        pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24); //-V2533 //-V2563
        pbuf[4] = LineCoding.format; //-V2563
        pbuf[5] = LineCoding.paritytype; //-V2563
        pbuf[6] = LineCoding.datatype; //-V2563
        /* Add your code here */
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

    case CDC_SEND_BREAK:
        /* Add your code here */
        break;    
    
    default:
        // здесь ничего
        break;
    }
  
    return (USBD_OK);
}



static int8_t CDC_Itf_Receive(uint8* buffer, uint *length) //-V2009 //-V2558
{
    SCPI::AddNewData(buffer, *length);

    USBD_CDC_ReceivePacket(reinterpret_cast<USBD_HandleTypeDef *>(HAL_USBD::handle));
    return (USBD_OK);
}
