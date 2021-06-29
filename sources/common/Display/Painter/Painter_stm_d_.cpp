// 2021/06/29 11:34:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter/Painter_.h"
#include "Display/Display.h"
#include "FDrive/FDrive.h"
#include "Settings/Settings.h"
#include <cstring>


static void CreateFileName(char name[255]);


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

    char fileName[255];

    CreateFileName(fileName);

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile((uint8 *)&bmFH, 14, &structForWrite);

    FDrive::WriteToFile((uint8 *)&bmIH, 40, &structForWrite);

    uint8 buffer[320 * 3] = { 0 };

    typedef struct tagRGBQUAD
    {
        uint8 blue;
        uint8 green;
        uint8 red;
        uint8 rgbReserverd;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for (int i = 0; i < 16; i++)
    {
        uint color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color));
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color));
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color));
        colorStruct.rgbReserverd = 0;
        ((RGBQUAD *)(buffer))[i] = colorStruct;
    }

    for (int i = 16; i < 32; i++)
    {
        colorStruct.blue = 0;
        colorStruct.green = 0;
        colorStruct.red = 0;
        colorStruct.rgbReserverd = 0;
        ((RGBQUAD *)(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    uint8 pixels[320];

    for (int row = 239; row >= 0; row--)
    {
        Display::ReadRow((uint8)row, pixels);

        FDrive::WriteToFile(pixels, 320, &structForWrite);
    }

    FDrive::CloseFile(&structForWrite);

    Display::Message::Show("Файл сохранён", "File saved");

    return true;
}


static void CreateFileName(char name[255])
{
    std::strcpy(name, "picture.bmp");
}
