#pragma once
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsCursors.h"


class Page;
class PageHelpItem;
struct ColorType;


#define COMMON_BEGIN_SB_EXIT  "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu"


const Page* PageForButton(Key::E button);   // Возвращает страницу меню, которая должна открываться по нажатию кнопки button.
bool IsMainPage(const void *item);                        // Возвращает true, если item - адрес главной страницы меню.


void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1);    // Рассчитывает условия отрисовки УГО малых кнопок управления выбором курсорами.
void DrawMenuCursTime(int x, int y, bool left, bool right);
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
};


struct PageChannelB
{
    static const Page *self;
    static void OnChanged_Input(bool);
    static void OnChanged_Couple(bool);
    static void OnChanged_Filtr(bool);
};


void  Cursors_Update();    // Вызываем эту функцию для каждого измерения, чтобы обновить положие курсоров, если они должны обновляться автоматически.


struct PageCursors
{
    struct PageSet
    {
        static const Page *self;
    };

    static const Page *self;
};


struct PageDebug
{
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


    static const Page *self;
};


struct PageDisplay
{
    static const Page *self;

    static ColorType *colorTypeGrid;

    static void OnPress_Accumulation_Clear();

    static void OnChanged_RefreshFPS(bool);

    static void OnChanged_Grid_Brightness();

    struct PageAccumulation
    {
        static const Page *self;
    };

    struct PageAveraging
    {
        static const Page *self;
    };

    struct PageGrid
    {
        static const Page *self;
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

    struct Tune
    {
        static void *pointer;
    };

    // Если true, то активен выбор типа измерений для выбора на странице ИЗМЕРЕНИЯ-НАСТРОИТЬ
    static bool choiceMeasuresIsActive;
};


struct PageService
{
    static const Page *self;

    struct PageMath
    {
        static const Page *self;

        struct PageFunction
        {
            static const Page *self;
        };

        struct PageFFT
        {
            struct Cursors
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
    struct Latest
    {
        static const Page *self;
    };

    struct Internal
    {
        static const Page *self;
    };

    struct PageExternal
    {
        static const Page *self;
    };

    struct SetMask
    {
        static const Page *self;
    };

    struct SetName
    {
        static const Page *self;
    };

    struct FileManager
    {
        static const Page *self;
    };

    static const Page *self;
};


void ChangeC_Memory_NumPoints(bool active);
void OnPressMemoryExtFileManager();
void Memory_SaveSignalToFlashDrive();
