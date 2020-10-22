#include "defines.h"
#include "common/Log_c.h"
#include "common/Hardware/HAL/HAL_c.h"
#include "common/Utils/Math_c.h"
#include "VCP/VCP.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


uint VCP::lastTimeSend = 0;
bool VCP::cableIsConnected = false;
bool VCP::connectToHost = false;


void VCP::Init()
{
    HAL_USBD::Init();
} 


void VCP::SendDataAsinch(pUCHAR buffer, int size)
{
    lastTimeSend = gTimerMS;

    const int SIZE_BUFFER = 64;
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::MinInt(size, SIZE_BUFFER);
    while (!HAL_USBD::PrevSendingComplete())  {};
    std::memcpy(trBuf, buffer, static_cast<uint>(size));

    HAL_USBD::Transmit(trBuf, size);
}

static const int SIZE_BUFFER_VCP = 256;     // WARN если поставить размер буфера 512, то на ТЕ207 глюки
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;

void VCP::Flush()
{
    if (sizeBuffer)
    {
        HAL_USBD::Flush(buffSend, sizeBuffer);
    }
    sizeBuffer = 0;
}

void VCP::SendDataSynch(pUCHAR buffer, int size)
{
    if (!VCP::connectToHost)
    {
        return;
    }

    lastTimeSend = gTimerMS;

    do 
    {
        if (sizeBuffer + size > SIZE_BUFFER_VCP)
        {
            int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
            LIMITATION(reqBytes, reqBytes, 0, size);
            HAL_USBD::Wait();
            std::memcpy(buffSend + sizeBuffer, buffer, static_cast<uint>(reqBytes));
            HAL_USBD::Transmit(buffSend, SIZE_BUFFER_VCP);
            size -= reqBytes;
            buffer += reqBytes;
            sizeBuffer = 0;
        }
        else
        {
            std::memcpy(buffSend + sizeBuffer, buffer, static_cast<uint>(size));
            sizeBuffer += size;
            size = 0;
        }
    } while (size);
}

void SendData(pUCHAR , int)
{

}

void VCP::SendStringAsinch(char *data)
{
    SendDataAsinch((uint8*)data, static_cast<int>(std::strlen(data)));
}

void VCP::SendStringSynch(char *data)
{
    SendDataSynch((uint8*)data, static_cast<int>(std::strlen(data)));
}

void VCP::SendFormatStringAsynch(char *format, ...)
{
    static const int SIZE_BUFFER = 200;
    static char buffer[SIZE_BUFFER];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\n");
    SendDataAsinch((uint8*)buffer, static_cast<int>(std::strlen(buffer)));
}

void VCP::SendFormatStringSynch(char *format, ...) {
    static const int SIZE_BUFFER = 200;
    char buffer[SIZE_BUFFER];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\n");
    SendDataSynch((uint8*)buffer, static_cast<int>(std::strlen(buffer)));
}


void VCP::Update()
{
    if(gTimerMS - lastTimeSend > 1000)
    {
        LOG_WRITE("Долго нет засылок");
    }
}
