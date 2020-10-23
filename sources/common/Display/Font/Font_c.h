#pragma once


struct Symbol
{
    uchar width;
    uchar bytes[8];

    struct S8
    {
        static const char FLASH_DRIVE = '\x80';         // Символ флешки внизу экрана
        static const char ETHERNET = '\x82';            // Символ Ethernet внизу экрана
        static const char USB = '\x84';                 // Символ USB внизу экрана
        static const char TPOS_1 = '\x8a';              // Символ TPos в окне памяти
        static const char TPOS_2 = '\x8b';              // 1-я часть символа TPos
        static const char TPOS_3 = '\x8a';              // 2-я часть символа TPos
        static const char TSHIFT_NORM_1 = '\x85';       // 1-я часть символа смещения по времени нормального
        static const char TSHIFT_NORM_2 = '\x84';       // 2-я часть символа смещения по времени нормального
        static const char TSHIFT_LEFT_1 = '\x94';       // 1-я часть символа смещения по времени слева от экрана
        static const char TSHIFT_LEFT_2 = '\x86';       // 2-я часть символа смещения по времени слева от экрана
        static const char TSHIFT_RIGHT_1 = '\x93';      // 1-я часть символа смещения по времени справа от экрана
        static const char TSHIFT_RIGHT_2 = '\x87';      // 2-я часть символа смещения по времени справа от экрана
        static const char PLAY = '\xae';                // Символ работы справа вверху экрана
        static const char TRIG_LEV_LOWER = '\x95';      // Символ уровень синхронизации ниже сетки
        static const char TRIG_LEV_ABOVE = '\x96';      // Символ уровень синхронизации выше сетки
        static const char TRIG_LEV_NORMAL = '\x94';     // Символ уровень синхронизации нормальный
        static const char RSHIFT_LOWER = '\x95';        // Символ смещения по напряжению ниже сетки
        static const char RSHIFT_ABOVE = '\x96';        // Символ смещения по напряжению выше сетки
        static const char RSHIFT_NORMAL = '\x93';       // Символ смещения по напряжению нормальный
        static const char GOVERNOR_SHIFT_0 = '\xaa';    // Символ регулятора
        static const char GOVERNOR_SHIFT_1 = '\xac';    // Символ регулятора
        static const char GOVERNOR_SHIFT_2 = '\x8c';    // Символ регулятора
        static const char GOVERNOR_SHIFT_3 = '\x0e';    // Символ регулятора
    };

    struct UGO2
    {
        static const char MEM_INT_SHOW_DIRECT = '\x6a'; // Кнопка рисовать текущий сигнал в режиме ВНУТР ЗУ
        static const char MEM_INT_SHOW_SAVED = '\x6c';  // Кнопка рисовать записанный сигнал в режиме ВНУТР ЗУ
        static const char BACKSPACE = '\x20';
        static const char DEL = '\x22';                 // ВНЕШН ЗУ-МАСКА-УДАЛИТЬ
        static const char INSERT = '\x26';              // ВНЕШН ЗУ-ИМЯ-ВСТАВИТЬ
        static const char TAB = '\x6e';
    };
};


struct TypeFont { enum E
{
	_5,
	_8,
	_UGO,
	_UGO2,
	_Count,
	_None
};};

struct Font
{
    int height;
    Symbol symbol[256];

    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol();
    static int GetLengthSymbol(uchar symbol);
    static void Set(TypeFont::E typeFont);

    static const Font *font;
    static const Font *fonts[TypeFont::_Count];

    static const uchar font5display[3080];
    static const uchar font8display[3080];
    static const uchar fontUGOdisplay[3080];
    static const uchar fontUGO2display[3080];
};
