#include "defines.h"
#include "main.h"
#include "common/Display/Colors_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Display.h"
#include "Settings/SettingsCommon.h"
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
static void DrawBigMNIPI();

uint8 *display_back_buffer = nullptr;
uint8 *display_back_buffer_end = nullptr;

void Display::Init()
{
    MainStruct::ms->display.value = 0.0F;
    MainStruct::ms->display.isRun = false;
    MainStruct::ms->display.timePrev = 0;
    MainStruct::ms->display.direction = 10.0F;

    for (int i = 0; i < 14; i++)
    {
        float red = (float)(i) / 14.0F * 31.0F + 0.5F;
        float green = (float)(i) / 14.0F * 63.0F + 0.5F;
        float blue = (float)(i) / 14.0F * 31.0F + 0.5F;
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
        Text("��� ��������� ������ ������� � ����������� ������ ������").DrawInCenterRect(0, 180, 320, 20);
        Text("����� ����������: ���./����. 8-017-270-02-00").DrawInCenterRect(0, 205, 320, 20);
        Text("������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-270-02-23").DrawInCenterRect(0, 220, 320, 20);
    }
    else if (MainStruct::ms->state == State::Mount)
    {
        uint dT = TIME_MS - MainStruct::ms->display.timePrev;
        DrawProgressBar(dT);
    }
    else if (MainStruct::ms->state == State::WrongFlash)
    {
        Text("�� ������� ��������� ����").DrawInCenterRect(0, 0, 320, 200, Color::FLASH_10);
        Text("���������, ��� �������� ������� FAT32").DrawInCenterRect(0, 20, 320, 200, Color::WHITE);
    }
    else if (MainStruct::ms->state == State::RequestAction)
    {
        Text("���������� ����������� �����������").DrawInCenterRect(0, 0, 320, 200);
        Text("���������� ���?").DrawInCenterRect(0, 20, 320, 200);

        DrawButton(290, 55, "��");
        DrawButton(290, 195, "���");
    }
    else if (MainStruct::ms->state == State::Upgrade)
    {
        Text("��������� ����������").DrawInCenterRect(0, 0, 320, 190);
        Text("��������� ������������ �����������").DrawInCenterRect(0, 0, 320, 220);

        int height = 30;
        int fullWidth = 280;
        int width = (int)((float)(fullWidth) * MainStruct::ms->percentUpdate);

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

    float step = (float)(dT) / MainStruct::ms->display.direction;

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

    Text("��������� USB-����.").DrawInCenterRect(X, y0, WIDTH, 10, Color::WHITE);
    Text("��� ����� ������������ �����������").DrawInCenterRect(X, y0 + dH, WIDTH, 10);
    Text("���������...").DrawInCenterRect(X, y0 + 2 * dH, WIDTH, 10);

    Rectangle(WIDTH, HEIGHT).Draw(X, Y);
    Region((int)(MainStruct::ms->display.value), HEIGHT).Fill(X, Y);
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

    int numColor = Math::Limitation((int)((float)(time) / (float)TIME_WAIT * 13.0F), 0, 13);

    Color((uint8)(numColor + 2)).SetAsCurrent();

    float amplitude = 3.0F - ((float)(time) / (TIME_WAIT / 2.0F)) * 3;
    Math::LimitBelow(&amplitude, 0.0F);
    float frequency = 0.05F;

    float radius = 5000.0F * (TIME_WAIT) / 3000.0F / (float)(time);
    Math::LimitBelow(&radius, 0.0F);

    float shift[240];

    for (int i = 0; i < 240; i++)
    {
        shift[i] = amplitude * std::sin(frequency * (float)(time) + (float)(i) / 5.0F);
    }

    for (int i = 0; i < numPoints; i++)
    {
        int x = (int)((float)(array[i].x) + shift[array[i].y]);
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
