// 2021/03/22 14:37:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef signed      char        int8;
typedef signed      short   int int16;
typedef unsigned    char        uint8;
typedef unsigned    short   int uint16;
typedef unsigned    int         uint;
typedef unsigned long long int  uint64;
typedef unsigned    char        uchar;
typedef const char             *pchar;
typedef const uint8            *puchar;
typedef const char *const       pstring;


typedef void  (*pFuncVV)();
typedef void  (*pFuncVII)(int, int);
typedef bool  (*pFuncBV)();
typedef void  (*pFuncVI)(int);
typedef void  (*pFuncVB)(bool);
typedef void  (*pFuncpCU8)(puchar);
typedef char* (*pFuncCFBC)(float, bool, char*);


inline void EmptyFuncVV() {};


template<class T>
struct ValueStruct
{
    ValueStruct(T v = (T)0) : value(v), valid(true) {}
    T value;
    void SetInvalid() { valid = false; }
    bool IsValid() const { return valid; }
    bool operator <(const T rhs) const { return value < rhs; }
    operator T() const { return value; }
private:
    bool valid;
};


struct Float : public ValueStruct<float>
{
    Float(float v = 0.0f) : ValueStruct(v) {}
};


struct InvalidFloat : public Float
{
    InvalidFloat() : Float() { SetInvalid(); }
};


struct Int : public ValueStruct<int>
{
    Int(int v = 0) : ValueStruct(v) {}
};


struct Uint8 : public ValueStruct<uint8>
{
    Uint8(uint8 v = 0) : ValueStruct(v) {}
};


struct Int16 : public ValueStruct<int16>
{
    Int16(int16 v = 0) : ValueStruct(v) {}
};


struct InvalidInt16 : public Int16
{
    InvalidInt16() : Int16() { SetInvalid(); }
};


#define _GET_BIT(value, bit) (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit) ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))
