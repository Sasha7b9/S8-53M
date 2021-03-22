// 2021/03/22 14:37:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


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
