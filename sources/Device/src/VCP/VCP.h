#pragma once


class VCP
{
public:
    static void Init();
    static void SendDataSynch(pUCHAR data, int size);
    static void SendStringSynch(char *data);                   // ��������� ������ ��� ������������ ����.
    static void SendFormatStringAsynch(char *format, ...);     // ��� ������ ��������� � ������������ ��������� \r\n.
    static void SendFormatStringSynch(char *format, ...);      // ��� ������ ��������� � ������������ ��������� \r\n.
    static void Flush();
    static void Update();

private:
    static void SendDataAsinch(pUCHAR buffer, int size);
    static void SendStringAsinch(char *data);
    // ����� ��������� ��������
    static uint lastTimeSend;
};
