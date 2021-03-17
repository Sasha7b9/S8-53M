#include "defines.h"
#include "main.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter_.h"
#include "common/Display/Primitives_.h"
#include "common/Display/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include <cmath>


using namespace Primitives;


struct Vector
{
    uint16 x;
    uint8 y;
};


int numPoints = 0;
#define SIZE_ARRAY 7000
Vector array[SIZE_ARRAY];


static void DrawProgressBar(uint dT);
static void DrawBigMNIPI();

uint8 *display_back_buffer = nullptr;

void Display::Init()
{
    MainStruct::ms->display.value = 0.0F;
    MainStruct::ms->display.isRun = false;
    MainStruct::ms->display.timePrev = 0;
    MainStruct::ms->display.direction = 10.0F;

    for (int i = 0; i < 14; i++)
    {
        float red = static_cast<float>(i) / 14.0F * 31.0F + 0.5F;
        float green = static_cast<float>(i) / 14.0F * 63.0F + 0.5F;
        float blue = static_cast<float>(i) / 14.0F * 31.0F + 0.5F;
        set.display.colors[i + 2] = Color::Make((uint8)red, (uint8)green, (uint8)blue);
    }

    Color::ResetFlash();

    Font::Set(TypeFont::S8);
}


void DrawButton(int x, int y, pchar text)
{
    int width = 25;
    int height = 20;
    Rectangle(width, height).Draw(x, y);
    Text(text).DrawInCenterRect(x, y, width + 2, height - 1);
}


void Display::Update()
{
    MainStruct::ms->display.isRun = true;

    MainStruct::ms->display.timePrev = TIME_MS;

    BeginFrame(Color::BLACK);

    Color::WHITE.SetAsCurrent();

    if (MainStruct::ms->state == State::Start || MainStruct::ms->state == State::Ok)
    {
        Display::BeginFrame(Color::BACK);
        Color::FILL.SetAsCurrent();
        Rectangle(319, 239).Draw(0, 0);
        DrawBigMNIPI();
        Color::WHITE.SetAsCurrent();
        Text("Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ").DrawInCenterRect(0, 180, 320, 20);
        Text("Отдел маркетинга: тел./факс. 8-017-270-02-00").DrawInCenterRect(0, 205, 320, 20);
        Text("Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-270-02-23").DrawInCenterRect(0, 220, 320, 20);
    }
    else if (MainStruct::ms->state == State::Mount)
    {
        uint dT = TIME_MS - MainStruct::ms->display.timePrev;
        DrawProgressBar(dT);
    }
    else if (MainStruct::ms->state == State::WrongFlash)
    {
        Text("НЕ УДАЛОСЬ ПРОЧИТАТЬ ДИСК").DrawInCenterRect(0, 0, 320, 200, Color::FLASH_10);
        Text("УБЕДИТЕСЬ, ЧТО ФАЙЛОВАЯ СИСТЕМА FAT32").DrawInCenterRect(0, 20, 320, 200, Color::WHITE);
    }
    else if (MainStruct::ms->state == State::RequestAction)
    {
        Text("Обнаружено программное обеспечение").DrawInCenterRect(0, 0, 320, 200);
        Text("Установить его?").DrawInCenterRect(0, 20, 320, 200);

        DrawButton(290, 55, "ДА");
        DrawButton(290, 195, "НЕТ");
    }
    else if (MainStruct::ms->state == State::Upgrade)
    {
        Text("Подождите завершения").DrawInCenterRect(0, 0, 320, 190);
        Text("установки программного обеспечения").DrawInCenterRect(0, 0, 320, 220);

        int height = 30;
        int fullWidth = 280;
        int width = static_cast<int>(static_cast<float>(fullWidth) * MainStruct::ms->percentUpdate);

        Region(width, height).Fill(20, 130);
        Rectangle(fullWidth, height).Draw(20, 130);
    }

    Display::EndFrame();
    MainStruct::ms->display.isRun = false;
}


void DrawProgressBar(uint dT)
{
    const int WIDTH = 300;
    const int HEIGHT = 20;
    const int X = 10;
    const int Y = 200;

    float step = static_cast<float>(dT) / MainStruct::ms->display.direction;

    MainStruct::ms->display.value += step;

    if (MainStruct::ms->display.direction > 0.0F && MainStruct::ms->display.value > WIDTH)
    {
        MainStruct::ms->display.direction = -MainStruct::ms->display.direction;
        MainStruct::ms->display.value -= step;
    }
    else if (MainStruct::ms->display.direction < 0.0F && MainStruct::ms->display.value < 0)
    {
        MainStruct::ms->display.direction = -MainStruct::ms->display.direction;
        MainStruct::ms->display.value -= step;
    }

    int dH = 15;
    int y0 = 50;

    Text("Обнаружен USB-диск.").DrawInCenterRect(X, y0, WIDTH, 10, Color::WHITE);
    Text("Идёт поиск программного обеспечения").DrawInCenterRect(X, y0 + dH, WIDTH, 10);
    Text("Подождите...").DrawInCenterRect(X, y0 + 2 * dH, WIDTH, 10);

    Rectangle(WIDTH, HEIGHT).Draw(X, Y);
    Region(static_cast<int>(MainStruct::ms->display.value), HEIGHT).Fill(X, Y);
}


bool Display::IsRun()
{
    return MainStruct::ms->display.isRun;
}


static void DrawBigMNIPI()
{
    static uint startTime = 0;
    static bool first = true;

    if (first)
    {
        first = false;
        startTime = TIME_MS;
    }

    uint time = TIME_MS - startTime;

    int numColor = 0;
    LIMITATION(numColor, static_cast<int>(static_cast<float>(time) / (float)TIME_WAIT * 13.0F), 0, 13);

    Color(static_cast<uint8>(numColor + 2)).SetAsCurrent();

    float amplitude = 3.0F - (static_cast<float>(time) / (TIME_WAIT / 2.0F)) * 3;
    LIMIT_BELOW(amplitude, 0.0F);
    float frequency = 0.05F;

    float radius = 5000.0F * (TIME_WAIT) / 3000.0F / static_cast<float>(time);
    LIMIT_BELOW(radius, 0);

    float shift[240];

    for (int i = 0; i < 240; i++)
    {
        shift[i] = amplitude * std::sin(frequency * static_cast<float>(time) + static_cast<float>(i) / 5.0F);
    }

    for (int i = 0; i < numPoints; i++)
    {
        int x = static_cast<int>(static_cast<float>(array[i].x) + shift[array[i].y]); //-V537
        int y = array[i].y;
        if (x > 0 && x < 319 && y > 0 && y < 239)
        {
            Point().Draw(x, y);
        }
    }
}


void Display::BeginFrame(const Color & /*color*/)
{

}


void Display::EndFrame()
{

}
