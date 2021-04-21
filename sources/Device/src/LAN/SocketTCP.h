// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

struct tcp_pcb;

static const int    DEFAULT_PORT = 7,
                    POLICY_PORT = 843;

struct TCPSocket
{
    static bool Init(void (*funcConnect)(), void (*funcReciever)(const char* buffer, uint length));

    static bool Send(const char* buffer, uint length);

    static void SendFormatString(char* format, ...);
};
