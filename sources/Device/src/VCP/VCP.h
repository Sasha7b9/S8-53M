#pragma once


class VCP
{
public:

    static void Init();

    static void SendDataSynch(puchar data, uint size);

    static void SendStringSynch(char *data);                   // Передаётся строка без завершающего нуля.

    static void SendFormatStringAsynch(char *format, ...);     // Эта строка передаётся с завершающими символами \r\n.

    static void SendFormatStringSynch(char *format, ...);      // Эта строка передаётся с завершающими символами \r\n.

    static void Flush();

    static void Update();

    static bool cableIsConnected;

    static bool connectToHost;

private:

    static void SendDataAsinch(puchar buffer, uint size);

    static void SendStringAsinch(char *data);

    // Время последней передачи
    static uint lastTimeSend;
};
