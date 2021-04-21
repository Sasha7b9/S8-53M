#define _STL_COMPILER_PREPROCESSOR 0
#include "defines.h"
#include "main.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"
#include "common/Hardware/Timer_.h"
#include "common/Hardware/HAL/HAL_.h"



static USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

#define APP_RX_DATA_SIZE  32
static uint8 UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */


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



static int8_t CDC_Itf_Init()
{
    USBD_CDC_SetRxBuffer(reinterpret_cast<USBD_HandleTypeDef *>(HAL_USBD::handle), UserRxBuffer);
    Timer::Enable(TypeTimer::Temp, 100, SetAttributeConnected);    // GOVNOCODE Задержка введена для того, чтобы не было ложных срабатываний в 
    return (USBD_OK);                                   // usbd_conf.c:HAL_PCD_SetupStageCallback при определении подключения хоста
}



static int8_t CDC_Itf_DeInit()
{
    VCP::cableIsConnected = false;
    VCP::connectToHost = false;

    return (USBD_OK);
}



static int8_t CDC_Itf_Control (uint8_t cmd, uint8_t *pbuf, uint16_t)
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
        LineCoding.bitrate    = (uint)(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
        LineCoding.format     = pbuf[4];
        LineCoding.paritytype = pbuf[5];
        LineCoding.datatype   = pbuf[6];
        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8)(LineCoding.bitrate);
        pbuf[1] = (uint8)(LineCoding.bitrate >> 8);
        pbuf[2] = (uint8)(LineCoding.bitrate >> 16);
        pbuf[3] = (uint8)(LineCoding.bitrate >> 24);
        pbuf[4] = LineCoding.format;
        pbuf[5] = LineCoding.paritytype;
        pbuf[6] = LineCoding.datatype;
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



static int8_t CDC_Itf_Receive(uint8_t * buffer, uint32_t *length)
{
    SCPI::AddNewData(buffer, *length);

    USBD_CDC_ReceivePacket(reinterpret_cast<USBD_HandleTypeDef *>(HAL_USBD::handle));
    return (USBD_OK);
}
