#pragma once


struct Symbol
{
    uchar width;
    uchar bytes[8];

    struct S8
    {
        static const char FLASH_DRIVE = '\x80';         // ������ ������ ����� ������
        static const char ETHERNET = '\x82';            // ������ Ethernet ����� ������
        static const char USB = '\x84';                 // ������ USB ����� ������
        static const char TPOS_1 = '\x8a';              // ������ TPos � ���� ������
        static const char TPOS_2 = '\x8b';              // 1-� ����� ������� TPos
        static const char TPOS_3 = '\x8a';              // 2-� ����� ������� TPos
        static const char TSHIFT_NORM_1 = '\x85';       // 1-� ����� ������� �������� �� ������� �����������
        static const char TSHIFT_NORM_2 = '\x84';       // 2-� ����� ������� �������� �� ������� �����������
        static const char TSHIFT_LEFT_1 = '\x94';       // 1-� ����� ������� �������� �� ������� ����� �� ������
        static const char TSHIFT_LEFT_2 = '\x86';       // 2-� ����� ������� �������� �� ������� ����� �� ������
        static const char TSHIFT_RIGHT_1 = '\x93';      // 1-� ����� ������� �������� �� ������� ������ �� ������
        static const char TSHIFT_RIGHT_2 = '\x87';      // 2-� ����� ������� �������� �� ������� ������ �� ������
        static const char PLAY = '\xae';                // ������ ������ ������ ������ ������
        static const char TRIG_LEV_LOWER = '\x95';      // ������ ������� ������������� ���� �����
        static const char TRIG_LEV_ABOVE = '\x96';      // ������ ������� ������������� ���� �����
        static const char TRIG_LEV_NORMAL = '\x94';     // ������ ������� ������������� ����������
        static const char RSHIFT_LOWER = '\x95';        // ������ �������� �� ���������� ���� �����
        static const char RSHIFT_ABOVE = '\x96';        // ������ �������� �� ���������� ���� �����
        static const char RSHIFT_NORMAL = '\x93';       // ������ �������� �� ���������� ����������
        static const char GOVERNOR_SHIFT_0 = '\xaa';    // ������ ����������
        static const char GOVERNOR_SHIFT_1 = '\xac';    // ������ ����������
        static const char GOVERNOR_SHIFT_2 = '\x8c';    // ������ ����������
        static const char GOVERNOR_SHIFT_3 = '\x0e';    // ������ ����������
    };

    struct UGO2
    {
        static const char MEM_INT_SHOW_DIRECT = '\x6a'; // ������ �������� ������� ������ � ������ ����� ��
        static const char MEM_INT_SHOW_SAVED = '\x6c';  // ������ �������� ���������� ������ � ������ ����� ��
        static const char BACKSPACE = '\x20';
        static const char DEL = '\x22';                 // ����� ��-�����-�������
        static const char INSERT = '\x26';              // ����� ��-���-��������
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
