// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/USBD/USBD_.h"
#include "common/Utils/Math_.h"
#include "VCP/VCP.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


uint VCP::lastTimeSend = 0;
bool VCP::cableIsConnected = false;
bool VCP::connectToHost = false;


void VCP::Init()
{
    USBD::Init();
} 


void VCP::SendDataAsinch(puchar buffer, uint size)
{
    lastTimeSend = TIME_MS;

    const uint SIZE_BUFFER = 64;
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::MinFrom2(size, SIZE_BUFFER);
    while (!USBD::PrevSendingComplete())  {};
    std::memcpy(trBuf, buffer, (uint)(size));

    USBD::Transmit(trBuf, size);
}

static const int SIZE_BUFFER_VCP = 256;     // WARN если поставить размер буфера 512, то на ТЕ207 глюки
static uint8 buffSend[SIZE_BUFFER_VCP];
static uint sizeBuffer = 0;

void VCP::Flush()
{
    if (sizeBuffer)
    {
        USBD::Flush(buffSend, sizeBuffer);
    }
    sizeBuffer = 0;
}

void VCP::SendDataSynch(puchar buffer, uint size)
{
    if (!VCP::connectToHost)
    {
        return;
    }

    lastTimeSend = TIME_MS;

    do 
    {
        if (sizeBuffer + size > SIZE_BUFFER_VCP)
        {
            int reqBytes = Math::Limitation(SIZE_BUFFER_VCP - (int)sizeBuffer, 0, (int)size);

            USBD::Wait();
            std::memcpy(buffSend + sizeBuffer, buffer, (uint)(reqBytes));
            USBD::Transmit(buffSend, SIZE_BUFFER_VCP);
            size -= reqBytes;
            buffer += reqBytes;
            sizeBuffer = 0;
        }
        else
        {
            std::memcpy(buffSend + sizeBuffer, buffer, (uint)(size));
            sizeBuffer += size;
            size = 0;
        }
    } while (size);
}

void SendData(puchar , int)
{

}

void VCP::SendStringAsinch(char *data)
{
    SendDataAsinch((uint8*)data, std::strlen(data));
}

void VCP::SendStringSynch(char *data)
{
    SendDataSynch((uint8*)data, std::strlen(data));
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
    SendDataAsinch((uint8*)buffer, std::strlen(buffer));
}

void VCP::SendFormatStringSynch(char *format, ...) {
    static const int SIZE_BUFFER = 200;
    char buffer[SIZE_BUFFER];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\n");
    SendDataSynch((uint8*)buffer, std::strlen(buffer));
}


void VCP::Update()
{
    if(TIME_MS - lastTimeSend > 1000)
    {
        LOG_WRITE("Долго нет засылок");
    }
}
