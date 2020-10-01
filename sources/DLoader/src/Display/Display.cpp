#include "defines.h"
#include "main.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "common/Hardware/HAL/HAL.h"
#include <cmath>


struct Vector
{
    uint16 x;
    uint8 y;
};


int numPoints = 0;
#define SIZE_ARRAY 7000
Vector array[SIZE_ARRAY];



static void DrawProgressBar(uint dT);
static void DrawBigMNIPI(void);
static void InitPoints(void);



void Display_Init(void)
{
    ms->display.value = 0.0F;
    ms->display.isRun = false;
    ms->display.timePrev = 0;
    ms->display.direction = 10.0F;

    gColorBack = Color::BLACK;
    gColorFill = Color::WHITE;

    for (int i = 0; i < 14; i++)
    {
        float red = static_cast<float>(i) / 14.0f * 31.0f + 0.5f;
        float green = static_cast<float>(i) / 14.0f * 63.0f + 0.5f;
        float blue = static_cast<float>(i) / 14.0f * 31.0f + 0.5f;
        set.display.colors[i + 2] = MAKE_COLOR((int)red, (int)green, (int)blue);
    }

    Painter::ResetFlash();

    Painter::LoadPalette(0);
    Painter::LoadPalette(1);
    Painter::LoadPalette(2);

    Painter::SetFont(TypeFont::_8);
    
    InitPoints();
}


void DrawButton(int x, int y, char *text)
{
    int width = 25;
    int height = 20;
    Painter::DrawRectangle(x, y, width, height);
    Painter::DrawStringInCenterRect(x, y, width + 2, height - 1, text);
}


void Display_Update(void)
{
    ms->display.isRun = true;

    uint dT = HAL_TIM2::TimeMS() - ms->display.timePrev;
    ms->display.timePrev = HAL_TIM2::TimeMS();

    Painter::BeginScene(Color::BLACK);

    Painter::SetColor(Color::WHITE);

    if (ms->state == State_Start || ms->state == State_Ok)
    {
        Painter::BeginScene(gColorBack);
        Painter::SetColor(gColorFill);
        Painter::DrawRectangle(0, 0, 319, 239);
        DrawBigMNIPI();
        Painter::SetColor(Color::WHITE);
        Painter::DrawStringInCenterRect(0, 180, 320, 20, "Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ");
        Painter::DrawStringInCenterRect(0, 205, 320, 20, "Отдел маркетинга: тел./факс. 8-017-270-02-00");
        Painter::DrawStringInCenterRect(0, 220, 320, 20, "Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-270-02-23");
    }
    else if (ms->state == State_Mount)
    {
        DrawProgressBar(dT);
    }
    else if (ms->state == State_WrongFlash)
    {
        Painter::DrawStringInCenterRectC(0, 0, 320, 200, "НЕ УДАЛОСЬ ПРОЧИТАТЬ ДИСК", Color::FLASH_10);
        Painter::DrawStringInCenterRectC(0, 20, 320, 200, "УБЕДИТЕСЬ, ЧТО ФАЙЛОВАЯ СИСТЕМА FAT32", Color::WHITE);
    }
    else if (ms->state == State_RequestAction)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 200, "Обнаружено программное обеспечение");
        Painter::DrawStringInCenterRect(0, 20, 320, 200, "Установить его?");

        DrawButton(290, 55, "ДА");
        DrawButton(290, 195, "НЕТ");
    }
    else if (ms->state == State_Upgrade)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 190, "Подождите завершения");
        Painter::DrawStringInCenterRect(0, 0, 320, 220, "установки программного обеспечения");

        int height = 30;
        int fullWidth = 280;
        int width = static_cast<int>(static_cast<float>(fullWidth) * ms->percentUpdate);

        Painter::FillRegion(20, 130, width, height);
        Painter::DrawRectangle(20, 130, fullWidth, height);
    }

    //DrawFrames();
    //DrawSeconds();

    Painter::EndScene();
    ms->display.isRun = false;
}


void DrawProgressBar(uint dT)
{
    const int WIDTH = 300;
    const int HEIGHT = 20;
    const int X = 10;
    const int Y = 200;

    float step = static_cast<float>(dT) / ms->display.direction;

    ms->display.value += step;

    if (ms->display.direction > 0.0F && ms->display.value > WIDTH)
    {
        ms->display.direction = -ms->display.direction;
        ms->display.value -= step;
    }
    else if (ms->display.direction < 0.0F && ms->display.value < 0)
    {
        ms->display.direction = -ms->display.direction;
        ms->display.value -= step;
    }

    int dH = 15;
    int y0 = 50;

    Painter::DrawStringInCenterRectC(X, y0, WIDTH, 10, "Обнаружен USB-диск.", Color::WHITE);
    Painter::DrawStringInCenterRect(X, y0 + dH, WIDTH, 10, "Идёт поиск программного обеспечения");
    Painter::DrawStringInCenterRect(X, y0 + 2 * dH, WIDTH, 10, "Подождите...");

    Painter::DrawRectangle(X, Y, WIDTH, HEIGHT);
    Painter::FillRegion(X, Y, static_cast<int>(ms->display.value), HEIGHT);
}


bool Display_IsRun(void)
{
    return ms->display.isRun;
}


static void DrawBigMNIPI(void)
{
    static uint startTime = 0;
    static bool first = true;

    if (first)
    {
        first = false;
        startTime = HAL_TIM2::TimeMS();
    }

    uint time = HAL_TIM2::TimeMS() - startTime;

    int numColor = 0;
    LIMITATION(numColor, static_cast<int>(static_cast<float>(time) / (float)TIME_WAIT * 13.0f), 0, 13);
    Painter::SetColor((Color::E)(numColor + 2));

    float amplitude = 3.0f - (static_cast<float>(time) / (TIME_WAIT / 2.0f)) * 3;
    LIMIT_BELOW(amplitude, 0.0F);
    float frequency = 0.05f;

    float radius = 5000.0F * (TIME_WAIT) / 3000.0F / static_cast<float>(time);
    LIMIT_BELOW(radius, 0);

    float shift[240];

    for (int i = 0; i < 240; i++)
    {
        shift[i] = amplitude * std::sin(frequency * static_cast<float>(time) + static_cast<float>(i) / 5.0f);
    }

    for (int i = 0; i < numPoints; i++)
    {
        int x = static_cast<int>(static_cast<float>(array[i].x) + shift[array[i].y]);
        int y = array[i].y;
        if (x > 0 && x < 319 && y > 0 && y < 239)
        {
            Painter::SetPoint(x, y);
        }
    }
}


static void InitPoints(void)
{
    uint8 buffer[320][240];

    Painter::DrawBigTextInBuffer(31, 70, 9, "МНИПИ", buffer);

    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            if (buffer[x][y])
            {
                array[numPoints].x = static_cast<uint16>(x);
                array[numPoints].y = static_cast<uint8>(y);
                numPoints++;
            }
        }
    }
}
