#pragma once
#include "defines.h"

 #define SOCKET_SEND(buffer, size) TCPSocket::Send(buffer, size)

static const int    DEFAULT_PORT = 7,
                    POLICY_PORT = 843;

class TCPSocket
{
public:
    static bool Init(void (*funcConnect)(), void (*funcReceiver)(const char *buffer, uint length));
    
    static bool Send(const char *buffer, int length);
    
    static void SendFormatString(char *format, ...);
};

void ETH_SendFormatString(char *format, ...);
