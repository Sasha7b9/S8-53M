// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Panel/Controls_.h"
#include "common/Settings/SettingsTypes_.h"


#define RETURN_TO_MAIN_MENU             0U
#define RETURN_TO_LAST_MEM              1U
#define RETURN_TO_INT_MEM               2U
#define RETURN_TO_DISABLE_MENU          3U


class Page;
class PageHelpItem;
struct ColorType;


#define COMMON_BEGIN_SB_EXIT  "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu"


const Page* PageForButton(Key::E key);   // ���������� �������� ����, ������� ������ ����������� �� ������� ������ key.
bool IsMainPage(const void *item);                        // ���������� true, ���� item - ����� ������� �������� ����.


void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1);    // ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void DrawSB_Exit(int x, int y);


struct PageMain
{
    static const Page *self;
};


struct PageChannelA
{
    static const Page *self;
    static void OnChanged_Input(bool);
    static void OnChanged_Couple(bool);
    static void OnChanged_Filtr(bool);
    static void OnPress_Balance();
};


struct PageChannelB
{
    static const Page *self;
    static void OnChanged_Input(bool);
    static void OnChanged_Couple(bool);
    static void OnChanged_Filtr(bool);
    static void OnPress_Balance();
};


void  Cursors_Update();    // �������� ��� ������� ��� ������� ���������, ����� �������� ������� ��������, ���� ��� ������ ����������� �������������.


struct PageCursors
{
    struct PageSet
    {
        static const Page *self;
    };

    // ������ ��������� �������� ���������� �� ������� ����
    static void DrawMenuCursorsVoltage(int x, int y, bool top, bool bottom);

    static void DrawMenuCursorsTime(int x, int y, bool left, bool right);


    static const Page *self;

private:

    static void CalculateXY(int *x0, int *x1, int *y0, int *y1);
};


struct PageDebug
{
    static const Page *self;

    static void LoadStretchADC(const Channel &);

    struct PageSerialNumber
    {
        static const Page *self;
    };

    struct PageConsole
    {
        static const Page *self;

        struct PageRegisters
        {
            static const Page *self;
        };
    };

    struct PageADC
    {
        static const Page *self;

        struct PageBalance
        {
            static const Page *self;
        };

        struct PageStretch
        {
            static const Page *self;
        };

        struct PageAltRShift
        {
            static const Page *self;
        };
    };

    struct PageRandomizer
    {
        static const Page *self;
    };

    struct PageInfo
    {
        static const Page *self;
    };
};


struct PageDisplay
{
    static const Page *self;

    static void OnChanged_RefreshFPS(bool);

    struct PageAccumulation
    {
        static const Page *self;

        static void OnPress_Clear();
    };

    struct PageAveraging
    {
        static const Page *self;
    };

    struct PageGrid
    {
        static ColorType colorType;

        static const Page *self;

        static void OnChangedBrightness();
    };

    struct PageSettings
    {
        static const Page *self;

        struct PageColors
        {
            static const Page *self;
        };
    };
};


struct PageMeasures
{
    static const Page *self;

    struct PageTune
    {
        static const Page *self;
    };

    // ���� true, �� ������� ����� ���� ��������� ��� ������ �� �������� ���������-���������
    static bool choiceMeasuresIsActive;
};


struct PageService
{
    static const Page *self;

    struct PageEthernet
    {
        static const Page * self;
    };

    struct PageMath
    {
        static const Page *self;

        struct PageFunction
        {
            static const Page *self;
        };

        struct PageFFT
        {
            static const Page *self;

            struct PageCursors
            {
                static const Page *self;
            };
        };
    };

    struct PageInformation
    {
        static const Page *self;
    };

    struct PageCalibrator
    {
        static const Page *self;
    };
};


struct PageTime
{
    static const Page *self;

    static void OnChanged_TPos(bool);

    static void OnChanged_PeakDet(bool);

    static bool InSelfRecoredMode();
};


struct PageTrig
{
    static const Page *self;

    static void OnPress_Mode(bool);

    struct PageAutoFind
    {
        static const Page *self;
    };
};


struct PageHelp
{
    static const Page *self;
};



struct PageMemory
{
    static uint exitFromModeSetNameTo;      // ���� ������������ �� ���� ��������� ����� ��� ���������� : 0 - � ��������
                                            // ����, 1 - � ���� ���������, 2 - � ���� ����� ��, 3 - � ������� ���� �
                                            // ����������� ����
    static bool needForSaveToFlashDrive;    // ���� 1, �� ����� ��������� ����� ��������� �� ������.
    static bool exitFromIntToLast;          // ���� 1, �� �������� �� �������� ���������� ������ ����� ��
                                            // ����������, � � ���� ���������

    static void OnChanged_NumPoints(bool active);

    struct PageLatest
    {
        static int16 currentSignal;     // ������� ����� ���������� ������� � ������ ������ - ���������

        static const Page *self;
    };

    struct PageInternal
    {
        static int8 currentSignal;      // ������� ����� �������, ����������� � ����
        static bool showAlways;         // ���� 1, �� ���������� ������ ��������� � ������ "�����. ��" ������

        static const Page *self;
    };

    struct PageExternal
    {
        static const Page *self;

        static void OnPress_FileManager();

        static void SaveSignalToFlashDrive();
    };

    struct PageSetMask
    {
        static const Page *self;

        static void OnRegSet(int angle, int maxIndex);
    };

    struct PageSetName
    {
        static const Page *self;
    };

    struct PageFileManager
    {
        static const Page *self;
    };

    static const Page *self;
};


