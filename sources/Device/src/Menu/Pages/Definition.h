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
    struct SerialNumber
    {
        static const Page *self;
    };

    static const Page *self;
};


struct PageDisplay
{
    static const Page *self;
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

    struct Math
    {
        struct Function
        {
            static const Page *self;
        };

        struct FFT
        {
            struct Cursors
            {
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


struct PageTime
{
    static const Page *self;
};


struct PageTrig
{
    static const Page *self;

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
extern const void *pMspFileManager;
