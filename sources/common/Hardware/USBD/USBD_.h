// 2021/06/17 14:49:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct USBD
{
    static void Init();
    static bool PrevSendingComplete();
    static void Transmit(uint8 *buffer, uint size);
    static void Flush(uint8 *buffer, uint sizeBuffer);
    static void Wait();
    static void *handle;    // USBD_HandleTypeDef
};
