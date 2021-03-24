// 2021/03/24 10:56:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Containers/String_.h"


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


struct Voltage : public Float
{
    Voltage(float v = 0.0f) : Float(v) {}

    String ToString(bool always_sign);
};


struct Time : public Float
{
    Time(float v = 0.0f) : Float(v) {}

    String ToString(bool always_sign);
};


struct Frequency : public Float
{
    Frequency(float v = 0.0f) : Float(v) {}

    String ToString();
};


struct Phase : public Float
{
    Phase(float v = 0.0f) : Float(v) {}

    String ToString();
};
