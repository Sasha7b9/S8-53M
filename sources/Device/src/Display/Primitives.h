#pragma once


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
        void Fill(int x, int y, Color::E color = Color::NUM);

    private:
        int width;
        int height;
    };

    class Rectangle
    {
    public:
        Rectangle(int w, int h) : width(w), height(h) { }
        void Draw(int x, int y, Color::E color = Color::NUM);
    private:
        int width;
        int height;
    };

    class HLine
    {
    public:
        HLine() { };
        void Draw(int y, int x1, int x2, Color::E color = Color::NUM);
    };

    class VLine
    {
    public:
        static void Draw(int x, int y0, int y1, Color::E color = Color::NUM);
    };

    // numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    // первой точки каждой линии соответствует очередному элементу массива x[]
    class MultiVPointLine
    {
    public:
        MultiVPointLine(int numLines, uint16 *x0, int delta, int count);
        void Draw(int y0, Color::E color = Color::NUM);
    private:
        int numLines;
        uint16 *x0;
        int delta;
        int count;
    };

    // \brief Нарисовать numLines горизонтальных линий, состоящих из count точек каждая с расстоянием между точками delta. Вертикальная координата
    // первой точки каждой линии соответствует очередному элементу массива y[]
    class MultiHPointLine
    {
    public:
        MultiHPointLine(int _numLines, const uint8 *_y, int _delta, int _count) : numLines(_numLines), y(_y), delta(_delta), count(_count) { }
        void Draw(int x, Color::E color = Color::NUM);
    private:
        int numLines;
        const uint8 *y;
        int delta;
        int count;
    };
}
