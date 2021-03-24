#pragma once


struct Color;


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char t);
    explicit String(pchar, ...);
    virtual ~String();

    void Set(pchar format, ...);
    void Set(const String &str);

    char *c_str() const;

//    int Draw(int x, int y, const Color &color) const;
    int Draw(int x, int y) const;

    static pstring _ERROR;

    void Free();

    void Append(pchar str);
    void Append(pchar str, uint numSymbols);
    void Append(char symbol);
    void Append(const String &str);
    
    uint Size() const;

    bool ToInt(int *out);

    char &operator[](uint i) const;

private:

    bool Allocate(uint size);

    uint NeedMemory(uint size);

    int DrawChar(int eX, int eY, uint8 symbol) const;

    char *buffer;

    uint capacity;

    static const uint SIZE_SEGMENT = 32;
};


class EmptyString : public String
{
public:
    EmptyString() : String("--.--") {}
};
