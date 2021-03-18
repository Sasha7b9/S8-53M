#pragma once


struct Color;


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char t);
    explicit String(pchar, ...);
    ~String();

    void Set(pchar format, ...);

    char *c_str() const;
    // Отобразить текст на экране в заданнх координатах
    int Draw(int x, int y, const Color &color) const;
    int Draw(int x, int y) const;

    static pString _ERROR; //-V2573

    void Free();

    void Append(pchar str);

    void Append(pchar str, uint numSymbols);

    void Append(char symbol);
    
    int Size() const;

    char &operator[](int i) const;

private:

    bool Allocate(uint size);

    char *buffer;
};
