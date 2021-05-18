// 2021/05/13 13:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Settings/SettingsCommon.h"
#include "Settings/SettingsNRST.h"


template void ROM::Settings<SettingsCommon>::Save(SettingsCommon *set);
template SettingsCommon *ROM::Settings<SettingsCommon>::Load();

template void ROM::Settings<SettingsNRST>::Save(SettingsNRST *nrst);
template SettingsNRST *ROM::Settings<SettingsNRST>::Load();


template<class T>
struct Packet
{
    uint size;

    // ���������� ����� ���������� �� ���� ������
    Packet *Next();

    // ����� ������� ����� � ������
    uint Begin() { return (uint)this; }

    // ����� ������� ����� �� ���� �������
    uint End() { return (uint)Next(); }

    // ���������� true, ���� � ������ ��� ������ (����� ��������� �� ��������� ���������� �������)
    bool IsEmpty() const;

    // ������� �������� � ����� ��������� � �������
    bool SaveSettings(T *nrst);
};


template<class T>
struct StructSector
{
    // ���������� ��������� �� ���������� ���������, ���� ������� ������� � nullptr � ��������� ������
    T *GetSaved() const;

    // ���������� ��������� �� �����, ������������� �� ������ �������
    Packet<T> *CreatePacket() const;

    // ���������� ��������� �� ��������� ���������� � ������� �����
    Packet<T> *LastPacket() const;

    // ������� �������� � ������ ��������� � �������
    bool SaveSettings(T *) const;

    static StructSector<T> &GetSector1();
    static StructSector<T> &GetSector2();

    const Sector &sector;
};


template<class T>
T *ROM::Settings<T>::Load()
{
    StructSector<T> &sector1 = StructSector<T>::GetSector1();
    StructSector<T> &sector2 = StructSector<T>::GetSector2();

    T *settings = sector2.GetSaved();

    if (settings)
    {
        return settings;
    }

    return sector1.GetSaved();
}


template<class T>
void ROM::Settings<T>::Save(T *settings)
{
    StructSector<T> &sector1 = StructSector<T>::GetSector1();
    StructSector<T> &sector2 = StructSector<T>::GetSector2();

    if (!sector1.SaveSettings(settings))
    {
        if (!sector2.SaveSettings(settings))
        {
            sector1.sector.Erase();
            sector1.SaveSettings(settings);
            sector2.sector.Erase();
        }
    }
}


template<class T>
bool StructSector<T>::SaveSettings(T *nrst) const
{
    Packet<T> *last = LastPacket();

    if (last == nullptr)
    {
        last = (Packet<T> *)sector.address;

        return last->SaveSettings(nrst);
    }

    return last->Next()->SaveSettings(nrst);
}


template<class T>
Packet<T> *StructSector<T>::LastPacket() const
{
    Packet<T> *packet = CreatePacket();

    while (!packet->IsEmpty())
    {
        // ���������, ��� ����� �� ��������� �� ������� �������. ���������� ������ �����, � ������� ���������� ��-
        // ��������, ��� ��������� ����� ������
        if (packet->End() >= sector.End() - 4)
        {
            return packet;
        }

        if (packet->Next()->IsEmpty())
        {
            return packet;
        }

        packet = packet->Next();
    }

    return nullptr;
}


template<class T>
StructSector<T> &StructSector<T>::GetSector1()
{
    static StructSector<T> sectorNRST1   = { HAL_ROM::sectors[Sector::_12_NRST_1] };
    static StructSector<T> sectorCommon1 = { HAL_ROM::sectors[Sector::_10_SETTINGS_1] };

    return (sizeof(T) == sizeof(SettingsNRST)) ? sectorNRST1 : sectorCommon1;
}


template<class T>
StructSector<T> &StructSector<T>::GetSector2()
{
    static StructSector<T> sectorNRST2   = { HAL_ROM::sectors[Sector::_13_NRST_2] };
    static StructSector<T> sectorCommon2 = { HAL_ROM::sectors[Sector::_11_SETTINGS_2] };

    return (sizeof(T) == sizeof(SettingsNRST)) ? sectorNRST2 : sectorCommon2;
}


template<class T>
Packet<T> *StructSector<T>::CreatePacket() const
{
    return (Packet<T> *)sector.address;
}


template<class T>
T *StructSector<T>::GetSaved() const
{
    Packet<T> *last = LastPacket();

    return (last && last->IsEmpty()) ? nullptr : (T *)last;
}


template<class T>
bool Packet<T>::SaveSettings(T *nrst)
{
    const Sector &sector = Sector::Get(Begin());

    if ((Begin() < sector.address) ||   // ��������� �� ����� �� ������ �������
        !IsEmpty()                 ||   // ��������� �� ��, ����� �� ����� ������ ������ �� ���� ��������
        (sizeof(T) + Begin() >= sector.End()))   // ��������� �� ����� �� ����� �������
    {
        return false;
    }

    nrst->size = sizeof(T);

    HAL_ROM::WriteBufferBytes(Begin(), nrst, sizeof(T));

    return true;
}


template<class T>
Packet<T> *Packet<T>::Next()
{
    if (size == (uint)(-1))
    {
        return nullptr;
    }

    return (Packet *)(Begin() + size);
}


template<class T>
bool Packet<T>::IsEmpty() const
{
    return (size == (uint)(-1));
}
