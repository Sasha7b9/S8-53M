#pragma once
#include "common/Keyboard/Controls_c.h"
#include "Settings/SettingsCursors.h"


class Page;


#define COMMON_BEGIN_SB_EXIT  "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu"


const Page* PageForButton(Key::E button);   // Возвращает страницу меню, которая должна открываться по нажатию кнопки button.
bool IsMainPage(const void *item);                        // Возвращает true, если item - адрес главной страницы меню.


void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1);    // Рассчитывает условия отрисовки УГО малых кнопок управления выбором курсорами.
void DrawMenuCursTime(int x, int y, bool left, bool right);
void DrawSB_Exit(int x, int y);


class PageMain
{
public:
    static const Page *self;
};


class PageChannelA
{
public:
    static const Page *self;
    static void OnChanged_Input(bool);
    static void OnChanged_Couple(bool);
    static void OnChanged_Filtr(bool);
};


class PageChannelB
{
public:
    static const Page *self;
    static void OnChanged_Input(bool);
    static void OnChanged_Couple(bool);
    static void OnChanged_Filtr(bool);
};


void  Cursors_Update();    // Вызываем эту функцию для каждого измерения, чтобы обновить положие курсоров, если они должны обновляться автоматически.


class PageCursors
{
public:

    class PageSet
    {
    public:
        static const Page *self;
    };

    static const Page *self;
};


class PageDebug
{
public:
    class SerialNumber
    {
    public:
        static const Page *self;
    };

    static const Page *self;
};


class PageDisplay
{
public:
    static const Page *self;
};


class PageMeasures
{
public:
    static const Page *self;

    class Tune
    {
    public:
        static void *pointer;
    };

    // Если true, то активен выбор типа измерений для выбора на странице ИЗМЕРЕНИЯ-НАСТРОИТЬ
    static bool choiceMeasuresIsActive;
};


class PageService
{
public:

    static const Page *self;

    class Math
    {
    public:

        class Function
        {
        public:
            static const Page *self;
        };

        class FFT
        {
        public:

            class Cursors
            {
            public:
                static const Page *self;
            };
        };
    };

    class Information
    {
    public:

        static const Page *self;
    };
};


class PageTime
{
public:
    static const Page *self;
};


class PageTrig
{
public:
    static const Page *self;
};


class PageHelp
{
public:
    static const Page *self;
};



class PageMemory
{
public:

    class Latest
    {
    public:
        static const Page *self;
    };

    class Internal
    {
    public:
        static const Page *self;
    };

    class SetMask
    {
    public:
        static const Page *self;
    };

    class SetName
    {
    public:
        static const Page *self;
    };

    class FileManager
    {
    public:
        static const Page *self;
    };

    static const Page *self;
};


void ChangeC_Memory_NumPoints(bool active);
void OnPressMemoryExtFileManager();
void Memory_SaveSignalToFlashDrive();
extern const void *pMspFileManager;
