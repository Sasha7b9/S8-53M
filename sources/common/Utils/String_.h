#pragma once


struct Color;


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(pchar format, ...);
    ~String();

    void Set(pchar format, ...);

    char *c_str() const;
    // Отобразить текст на экране в заданнх координатах
    int Draw(int x, int y, const Color &color) const;
    int Draw(int x, int y) const;

    static pString _ERROR; //-V2573

    void Free();

    void Append(pchar str);

    void Append(pchar str, int numSymbols);

    void Append(char symbol);
    
    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(int numSymbols);

    void RemoveFromEnd();

    int Size() const;

    char &operator[](int i) const;

private:

    bool Allocate(int size);

    char *buffer;
};
