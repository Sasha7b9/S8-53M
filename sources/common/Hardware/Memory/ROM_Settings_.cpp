// 2021/05/13 13:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


//template void ROM::Settings<::Settings>::Save(::Settings *set);

template void ROM::Settings<SettingsNRST>::Save(SettingsNRST *nrst);
template SettingsNRST *ROM::Settings<SettingsNRST>::GetSaved();


struct Packet
{
    uint size;

    // ¬озвращает адрес следующего за этим пакета
    Packet *Next();

    // јдрес первого байта в пакете
    uint Begin() { return (uint)this; }

    // јдрес первого байта за этим пакетом
    uint End() { return (uint)Next(); }

    // ¬озвращает true, если в пакете нет данных (пакет находитс€ за последним записанным пакетом)
    bool IsEmpty() const;

    // ѕопытка записать в пакет структуру с данными
    bool SaveSettings(SettingsNRST *nrst);
};


struct StructSector
{
    // ¬озвращает указатель на сохранЄнную структуру, если такова€ имеетс€ и nullptr в противном случае
    SettingsNRST *GetSaved() const;

    // ¬озвращает указатель на пакет, установленный на начало сектора
    Packet *CreatePacket() const;

    // ¬озвращает указатель на последний записанный в секторе пакет
    Packet *LastPacket() const;

    // ѕопытка записать в сектор структуру с данными
    bool SaveSettings(SettingsNRST *) const;

    const Sector &sector;
};


static const StructSector sector1 = { HAL_ROM::sectors[Sector::_03_NRST_1] };
static const StructSector sector2 = { HAL_ROM::sectors[Sector::_04_NRST_2] };


template<class T>
T *ROM::Settings<T>::GetSaved()
{
    T *settings = sector2.GetSaved();

    if (settings)
    {
        return settings;
    }

    return sector1.GetSaved();
}


template<class T>
void ROM::Settings<T>::Save(T *nrst)
{
    if (!sector1.SaveSettings(nrst))
    {
        if (!sector2.SaveSettings(nrst))
        {
            sector1.sector.Erase();
            sector1.SaveSettings(nrst);
            sector2.sector.Erase();
        }
    }
}


bool StructSector::SaveSettings(SettingsNRST *nrst) const
{
    Packet *last = LastPacket();

    if (last == nullptr)
    {
        last = (Packet *)sector.address;

        return last->SaveSettings(nrst);
    }

    return last->Next()->SaveSettings(nrst);
}


Packet *StructSector::LastPacket() const
{
    Packet *packet = CreatePacket();

    while (!packet->IsEmpty())
    {
        // ѕровер€ем, что пакет не находитс€ на границе сектора. ќткидываем четыре байта, в которых поместитс€ ин-
        // формаци€, что следующий пакет пустой
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


Packet *StructSector::CreatePacket() const
{
    return (Packet *)sector.address;
}


SettingsNRST *StructSector::GetSaved() const
{
    Packet *last = LastPacket();

    return (last && last->IsEmpty()) ? nullptr : (SettingsNRST *)last;
}


bool Packet::SaveSettings(SettingsNRST *nrst)
{
    const Sector &sector = Sector::Get(Begin());

    if ((Begin() < sector.address) ||   // ѕровер€ем на выход за начало сектора
        !IsEmpty()                 ||   // ѕровер€ем на то, чтобы по этому адресу ничего не было записано
        (sizeof(SettingsNRST) + Begin() >= sector.End()))   // ѕровер€ем на выход за конец сектора
    {
        return false;
    }

    nrst->size = sizeof(SettingsNRST);

    HAL_ROM::WriteBufferBytes(Begin(), nrst, sizeof(SettingsNRST));

    return true;
}


Packet *Packet::Next()
{
    if (size == (uint)(-1))
    {
        return nullptr;
    }

    return (Packet *)(Begin() + size);
}


bool Packet::IsEmpty() const
{
    return (size == (uint)(-1));
}
