// 2021/06/29 11:34:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "FDrive/FDrive.h"


bool Painter::SaveScreenToFlashDrive()
{
    if (!FDrive::IsConnected())
    {
        return false;
    }

#pragma pack(1)

    struct BITMAPFILEHEADER
    {
        char   type0;   // 0
        char   type1;   // 1
        uint   size;    // 2
        uint16 res1;    // 6
        uint16 res2;    // 8
        uint   offBits; // 10
    }
    bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240,
        0,
        0,
        14 + 40 + 1024
    };

    // 14

    struct BITMAPINFOHEADER
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    }
    bmIH =
    {
        40,     // size;
        320,    // width;
        240,    // height;
        1,      // planes;
        8,      // bitCount;
        0,      // compression;
        0,      // sizeImage;
        0,      // xPelsPerMeter;
        0,      // yPelsPerMeter;
        0,      // clrUsed;
        0       // clrImportant;
    };

    // 54

#pragma pack(4)

    StructForWrite structForWrite;

    return true;
}
