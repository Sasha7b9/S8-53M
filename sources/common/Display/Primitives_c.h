#pragma once
#include "common/Utils/String_c.h"


namespace Primitives
{
    class Point
    {
    public:
        void Draw(int x, int y);
    };

    class Region
    {
    public:
        Region(int w, int h) : width(w), height(h) { }
        void Fill(int x, int y, Color::E color = Color::Count);

    private:
        int width;
        int height;
    };

    class Rectangle
    {
    public:
        Rectangle(int w, int h) : width(w), height(h) { }
        void Draw(int x, int y, Color::E color = Color::Count);
    private:
        int width;
        int height;
    };

    class HLine
    {
    public:
        HLine() { };
        void Draw(int y, int x1, int x2, Color::E color = Color::Count);
    };

    class VLine
    {
    public:
        VLine() { };
        void Draw(int x, int y0, int y1, Color::E color = Color::Count);
    };

    // numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. �������������� ����������
    // ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
    class MultiVPointLine
    {
    public:
        MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) : numLines(_numLines), x0(_x0), delta(_delta), count(_count) { }
        void Draw(int y0, Color::E color = Color::Count);
    private:
        int numLines;
        uint16 *x0;
        int delta;
        int count;
        // ���������� ���� ������������ ������ �� count ����� c ����������� delta ����� ���������� �������
        static void DrawVPointLine(int x, int y, int count, int delta);
    };

    // \brief ���������� numLines �������������� �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. ������������ ����������
    // ������ ����� ������ ����� ������������� ���������� �������� ������� y[]
    class MultiHPointLine
    {
    public:
        MultiHPointLine(int _numLines, const uint8 *_y, int _delta, int _count) : numLines(_numLines), y(_y), delta(_delta), count(_count) { }
        void Draw(int x, Color::E color = Color::Count);
    private:
        int numLines;
        const uint8 *y;
        int delta;
        int count;
        // ���������� ���� �������������� ������ �� count ����� c ����������� delta ����� ���������� �������
        static void DrawHPointLine(int x, int y, int count, int delta);
    };

    class VPointLine
    {
    public:
        VPointLine(float d) : delta(d) { }
        void Draw(int x, int y0, int y1, Color::E color = Color::Count);
    private:
        float delta;
    };

    class HPointLine
    {
    public:
        HPointLine(float d) : delta(d) { }
        void Draw(int y, int x0, int x1);
    private:
        float delta;
    };


    class Line
    {
    public:
        void Draw(int x0, int y0, int x1, int y1, Color::E color = Color::Count);
    };


    // ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. dStart ��������� �������� ������ �������� �����
    // ������������ ������ ������.
    class DashedHLine
    {
    public:
        DashedHLine(int dFill, int dEmpty) : fill(dFill), empty(dEmpty) { }
        void Draw(int y, int x0, int x1, int dStart);
    private:
        int fill;
        int empty;
    };

    //  ������ ����������� ������������ �����.
    class DashedVLine
    {
    public:
        DashedVLine(int dFill, int dEmpty) : fill(dFill), empty(dEmpty) { }
        void Draw(int x, int y0, int y1, int dStart);
    private:
        int fill;
        int empty;
    };
}
