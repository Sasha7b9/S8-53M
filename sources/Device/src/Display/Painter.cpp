#include "defines.h"
#include "Colors.h"
#include "Painter.h"
#include "font/Font.h"
#include "common/Log.h"
#include "Ethernet/Ethernet.h"
#include "Ethernet/TcpSocket.h"
#include "Hardware/Timer.h"
#include "common/Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "Utils/Math.h"
#include "Utils/GlobalFunctions.h"
#include "FDrive/FDrive.h"
#include "Menu/FileManager.h"



static bool inverseColors = false;
static Color::E currentColor = Color::NUM;

static enum StateTransmit
{
    StateTransmit_Free,
    StateTransmit_NeedForTransmitFirst,  // Это когда нужно передать первый кадр - передаются шрифты
    StateTransmit_NeedForTransmitSecond, // Это когда нужно передать второй и последующий кадры - шрифты не передаются
    StateTransmit_InProcess
} stateTransmit = StateTransmit_Free;

static bool noFonts = false;



void Painter::SendFrame(bool first, bool noFonts_)
{
    noFonts = noFonts_;

    if (stateTransmit == StateTransmit_Free)
    {
        stateTransmit = (first ? StateTransmit_NeedForTransmitFirst : StateTransmit_NeedForTransmitSecond);
    }
}


void Painter::CalculateCurrentColor()
{
    if (currentColor == Color::FLASH_10)
    {
        SetColor(inverseColors ? COLOR_BACK : COLOR_FILL);
    }
    else if (currentColor == Color::FLASH_01)
    {
        SetColor(inverseColors ? COLOR_FILL : COLOR_BACK);
    }
}


void CalculateColor(uint8 *color)
{
    currentColor = (Color::E)*color;
    if (*color == Color::FLASH_10)
    {
        *color = static_cast<uint8>(inverseColors ? COLOR_BACK : COLOR_FILL);
    }
    else if (*color == Color::FLASH_01)
    {
        *color = static_cast<uint8>(inverseColors ? COLOR_FILL : COLOR_BACK);
    }
}


void InverseColor(Color::E *color)
{
    *color = (*color == Color::BLACK) ? Color::WHITE : Color::BLACK;
}


static void OnTimerFlashDisplay()
{
    inverseColors = !inverseColors;
}


void Painter::ResetFlash()
{
    Timer::Enable(kFlashDisplay, 400, OnTimerFlashDisplay);
    inverseColors = false;
}


void Painter::DrawRectangle(int x, int y, int width, int height)
{
    DrawHLine(y, x, x + width);
    DrawVLine(x, y, y + height);
    DrawHLine(y + height, x, x + width);
    if (x + width < SCREEN_WIDTH)
    {
        DrawVLine(x + width, y, y + height);
    }
}


void Painter::DrawRectangleC(int x, int y, int width, int height, Color::E color)
{
    SetColor(color);
    DrawRectangle(x, y, width, height);
}


void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}


void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}


static int numberColorsUsed = 0;



void Painter::SendToVCP(uint8 *pointer, int size)
{
    if(stateTransmit == StateTransmit_InProcess)
    {
        VCP::SendDataSynch(pointer, size);
        TCPSocket_Send((const char *)pointer, (uint)size);
    }
}


void Painter::SendToDisplay(uint8 *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; i += 4)
    {
        while (pinDisplayReady.Read() == 0) { };
        Timer::PauseOnTicks(100);
        *HAL_FSMC::ADDR_CDISPLAY = *bytes++;
        *HAL_FSMC::ADDR_CDISPLAY = *bytes++;
        *HAL_FSMC::ADDR_CDISPLAY = *bytes++;
        *HAL_FSMC::ADDR_CDISPLAY = *bytes++;
    }
}


static void Get4Bytes(uint8 bytes[4])
{
    while (pinDisplayReady.Read() == 0) { }
    bytes[0] = *HAL_FSMC::ADDR_CDISPLAY;
    bytes[1] = *HAL_FSMC::ADDR_CDISPLAY;
    bytes[2] = *HAL_FSMC::ADDR_CDISPLAY;
    bytes[3] = *HAL_FSMC::ADDR_CDISPLAY;
}


void Painter::SetPalette(Color::E color)
{
    uint8 command[4];
    command[0] = SET_PALETTE;
    *(command + 1) = static_cast<uint8>(color);
    *((uint16*)(command + 2)) = set.display.colors[color];
    SendToDisplay(command, 4);
    SendToVCP(command, 4);
}


void Painter::SetColor(Color::E color)
{
    if (color != currentColor)
    {
        currentColor = color;
        if (currentColor > Color::NUM)
        {
            CalculateColor((uint8 *)&color);
        }
        uint8 command[4] = {SET_COLOR};
        command[1] = static_cast<uint8>(color);
        SendToDisplay(command, 4);
        SendToVCP(command, 2);
    }
}


Color::E Painter::CurrentColor(void)
{
    return currentColor;
}


void Painter::DrawHLine(int y, int x0, int x1)
{
    CalculateCurrentColor();
    uint8 command[8];
    command[0] = DRAW_HLINE;
    *(command + 1) = (uint8)y;
    *((int16*)(command + 2)) = (int16)x0;
    *((int16*)(command + 4)) = (int16)x1;
    SendToDisplay(command, 8);
    SendToVCP(command, 6);
}


void Painter::DrawHLineC(int y, int x0, int x1, Color::E color)
{
    SetColor(color);
    DrawHLine(y, x0, x1);
}


void Painter::DrawVLine(int x, int y0, int y1)
{
    CalculateCurrentColor();
    uint8 command[8];
    command[0] = DRAW_VLINE;
    *((int16*)(command + 1)) = (int16)x;
    *(command + 3) = (uint8)y0;
    *(command + 4) = (uint8)y1;
    SendToDisplay(command, 8);
    SendToVCP(command, 5);
}


void Painter::DrawVLineC(int x, int y0, int y1, Color::E color)
{
    SetColor(color);
    DrawVLine(x, y0, y1);
}


void Painter::DrawLineC(int x0, int y0, int x1, int y1, Color::E color)
{
    SetColor(color);
    DrawLine(x0, y0, x1, y1);
}


void Painter::DrawVPointLine(int x, int y0, int y1, float delta, Color::E color)
{
    SetColor(color);
    uint8 numPoints = static_cast<uint8>((y1 - y0) / delta);
    uint8 command[6];
    command[0] = DRAW_VPOINT_LINE;
    *((int16*)(command + 1)) = (int16)x;
    *(command + 3) = (uint8)y0;
    *(command + 4) = static_cast<uint8>(delta);
    *(command + 5) = (uint8)numPoints;
    SendToDisplay(command, 6);
}


void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += static_cast<int>(delta))
    {
        SetPoint(x, y);
    }
}


void Painter::SetPoint(int x, int y)
{
    uint8 command[4];
    command[0] = SET_POINT;
    *((int16*)(command + 1)) = (int16)x;
    *(command + 3) = (uint8)y;
    SendToDisplay(command, 4);
    SendToVCP(command, 4);
}


void Painter::DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color::E color) 
{
    if(numLines > 20) 
    {
        LOG_ERROR("Число линий слишком большое %d", numLines);
        return;
    }

    SetColor(color);
    uint8 command[60];
    command[0] = DRAW_MULTI_VPOINT_LINES;
    *(command + 1) = (uint8)numLines;
    *(command + 2) = (uint8)y;
    *(command + 3) = (uint8)count;
    *(command + 4) = (uint8)delta;
    *(command + 5) = 0;
    uint8 *pointer = command + 6;
    for(int i = 0; i < numLines; i++) 
    {
        *((uint16*)pointer) = x[i];
        pointer += 2;
    }
    int numBytes = 1 + 1 + 1 + numLines * 2 + 1 + 1;
    while(numBytes % 4) 
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToVCP(command, 1 + 1 + 1 + 1 + 1 + 1 + numLines * 2);
}


void Painter::DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color::E color)
{
    if (numLines > 20)
    {
        LOG_ERROR("Число линий слишком большое %d", numLines);
        return;
    }
    SetColor(color);
    uint8 command[30];
    command[0] = DRAW_MULTI_HPOINT_LINES_2;
    *(command + 1) = (uint8)numLines;
    *((uint16*)(command + 2)) = (uint16)x;
    *(command + 4) = (uint8)count;
    *(command + 5) = (uint8)delta;
    uint8 *pointer = command + 6;
    for (int i = 0; i < numLines; i++)
    {
        *pointer = y[i];
        pointer++;
    }
    int numBytes = 1 +     // command
        1 +     // numLines
        2 +     // x
        numLines +    // numLines
        1 +
        1;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToVCP(command, 1 + 1 + 2 + 1 + 1 + numLines);
}


void Painter::DrawLine(int x0, int y0, int x1, int y1)
{
    if (x0 == x1)
    {
        DrawVLine(x0, y0, y1);
    }
    else if (y0 == y1)
    {
        DrawHLine(y0, x0, x1);
    }
}


void Painter::FillRegion(int x, int y, int width, int height)
{
    CalculateCurrentColor();
    uint8 command[8];
    command[0] = FILL_REGION;
    *((int16*)(command + 1)) = (int16)x;
    *(command + 3) = (uint8)y;
    *((int16*)(command + 4)) = (int16)width;
    *(command + 6) = (uint8)height;
    SendToDisplay(command, 8);
    SendToVCP(command, 7);
}


void Painter::FillRegionC(int x, int y, int width, int height, Color::E color)
{
    SetColor(color);
    FillRegion(x, y, width, height);
}


void Painter::DrawVolumeButton(int x, int y, int width, int height, int thickness, Color::E normal, Color::E bright, Color::E dark, bool isPressed, bool inShade)
{
    if (inShade)
    {
        thickness = 1;
    }
    FillRegionC(x + thickness, y + thickness, width - thickness * 2, height - thickness * 2, normal);
    if (isPressed || inShade)
    {
        for (int i = 0; i < thickness; i++)
        {
            DrawHLineC(y + i, x + i, x + width - i, dark);
            DrawVLine(x + i, y + 1 + i, y + height - i);
            DrawVLineC(x + width - i, y + 1 + i, y + height - i, bright);
            DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            DrawHLineC(y + i, x + i, x + width - i, bright);
            DrawVLine(x + i, y + 1 + i, y + height - i);
            DrawVLineC(x + width - i, y + 1 + i, y + height - i, dark);
            DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
}


void Painter::SetBrightnessDisplay(int16 brightness)
{
    float recValue = 1601.0f;
    if (brightness < 100)
    {
        recValue = 64.0f + (600.0f - 63.0f) / 100.0f / 100.0f * brightness * brightness;
    }
    uint8 command[4];
    command[0] = SET_BRIGHTNESS;
    *((uint16*)(command + 1)) = (uint16)recValue;
    SendToDisplay(command, 4);
}


int NumberColorsInSceneCol(void)
{
    return numberColorsUsed;
}


void Painter::DrawVLineArray(int x, int numLines, uint8 *y0y1, Color::E color)
{
    SetColor(color);
    uint8 command[255 * 2 + 4 + 4];
    command[0] = DRAW_VLINES_ARRAY;
    *((int16*)(command + 1)) = (int16)x;
    if (numLines > 255)
    {
        numLines = 255;
    }
    *(command + 3) = (uint8)numLines;
    for (int i = 0; i < numLines; i++)
    {
        *(command + 4 + i * 2) = *(y0y1 + i * 2);
        *(command + 4 + i * 2 + 1) = *(y0y1 + i * 2 + 1);
    }
    int numBytes = numLines * 2 + 4;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToVCP(command, 1 + 2 + 1 + 2 * numLines);
}


void Painter::DrawSignal(int x, uint8 data[281], bool modeLines)
{
    uint8 command[284];
    command[0] = static_cast<uint8>(modeLines ? DRAW_SIGNAL_LINES : DRAW_SIGNAL_POINTS);
    *((int16*)(command + 1)) = (int16)x;
    for (int i = 0; i < 281; i++)
    {
        *(command + 3 + i) = data[i];
    }
    SendToDisplay(command, 284);
    SendToVCP(command, 284);
}


void Painter::LoadPalette(void)
{
    for (int i = 0; i < Color::NUM; i++)
    {
        SetPalette((Color::E)i);
    }
}


void Painter::BeginScene(Color::E color)
{
    if (stateTransmit == StateTransmit_NeedForTransmitFirst || stateTransmit == StateTransmit_NeedForTransmitSecond)
    {
        bool needForLoadFontsAndPalette = stateTransmit == StateTransmit_NeedForTransmitFirst;
        stateTransmit = StateTransmit_InProcess;
        if(needForLoadFontsAndPalette) 
        {
            LoadPalette();
            if(!noFonts)                // Если был запрос на загрузку шрифтов
            {
                LoadFont(TypeFont::_5);
                LoadFont(TypeFont::_8);
                LoadFont(TypeFont::_UGO);
                LoadFont(TypeFont::_UGO2);
            }
        }
    }

    FillRegionC(0, 0, 319, 239, color);
}


void Painter::RunDisplay()
{
    uint8 command[4];
    command[0] = RUN_BUFFER;
    SendToDisplay(command, 4);
}


void Painter::EndScene(bool endScene)
{
    if (FRAMES_ELAPSED != 1)
    {
        FRAMES_ELAPSED = 1;
        return;
    }
    uint8 command[4];
    command[0] = END_SCENE;

    if (endScene)
    {
        SendToDisplay(command, 4);
        SendToVCP(command, 1);
    }
    if (stateTransmit == StateTransmit_InProcess)
    {
        VCP::Flush();
        stateTransmit = StateTransmit_Free;
    }

    RunDisplay();
}


Color::E Painter::GetColor(int x, int y)
{
    uint8 command[4];
    command[0] = GET_POINT;
    *((int16*)(command + 1)) = (int16)x;
    *(command + 3) = (uint8)y;
    SendToDisplay(command, 4);

    Get4Bytes(command);

    return (Color::E)(command[0] & 0x0f);
}


void Painter::Get8Points(int x, int y, uint8 buffer[4])
{
    uint8 command[4];
    command[0] = GET_POINT;
    *((int16*)(command + 1)) = (int16)x;
    *(command + 3) = (uint8)y;
    SendToDisplay(command, 4);
    Get4Bytes(buffer);
}


uint8 Get2Points(int x, int y)
{
    while(pinDisplayReady.Read() == 0) { }
    *HAL_FSMC::ADDR_CDISPLAY = GET_POINT;
    *HAL_FSMC::ADDR_CDISPLAY = (uint8)x;
    *HAL_FSMC::ADDR_CDISPLAY = (uint8)(x >> 8);
    *HAL_FSMC::ADDR_CDISPLAY = (uint8)y;
    while(pinDisplayReady.Read() == 0) { }
    return *HAL_FSMC::ADDR_CDISPLAY;
}


void Painter::DrawPicture(int x, int y, int width, int height, uint8 *address)
{
    uint8 command[4];
    command[0] = DRAW_PICTURE;
    *((uint16*)(command + 1)) = (uint16)x;
    *(command + 3) = (uint8)y;
    SendToDisplay(command, 4);
    *((uint16*)(command)) = (uint16)width;
    *(command + 2) = (uint8)height;
    *(command + 3) = 0;
    SendToDisplay(command, 4);
    for (int i = 0; i < width * height / 2 / 4; i++)
    {
        *(command) = *address++;
        *(command + 1) = *address++;
        *(command + 2) = *address++;
        *(command + 3) = *address++;
        SendToDisplay(command, 4);
    }
}


uint16 Painter::ReduceBrightness(uint16 colorValue, float newBrightness)
{
    int red = static_cast<int>(R_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(red, red, 0, 31);
    int green = static_cast<int>(G_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(green, green, 0, 63);
    int blue = static_cast<int>(B_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(blue, blue, 0, 31);
    return MAKE_COLOR(red, green, blue);
}


bool Painter::SaveScreenToFlashDrive(void)
{
    return true;
}
