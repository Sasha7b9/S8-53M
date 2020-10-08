#pragma once
#include "Utils/String.h"


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
        VLine() { };
        void Draw(int x, int y0, int y1, Color::E color = Color::NUM);
    };

    class Text
    {
    public:
        Text(char* t) : text(t) {}
        Text(pString t) : text(t) {};
        int Draw(int x, int y, Color::E color = Color::NUM);
    private:
        String text;
    };

    // numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    // первой точки каждой линии соответствует очередному элементу массива x[]
    class MultiVPointLine
    {
    public:
        MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) : numLines(_numLines), x0(_x0), delta(_delta), count(_count) { }
        void Draw(int y0, Color::E color = Color::NUM);
    private:
        int numLines;
        uint16 *x0;
        int delta;
        int count;
        // Нарисовать одну вертикальную лиинию из count точек c расстоянием delta между соседнимит точками
        static void DrawVPointLine(int x, int y, int count, int delta);
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
        // Нарисовать одну горизонтальную лиинию из count точек c расстоянием delta между соседнимит точками
        static void DrawHPointLine(int x, int y, int count, int delta);
    };
}
