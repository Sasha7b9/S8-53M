// 2021/05/13 13:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Settings/SettingsNRST.h"


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
    bool SaveSettings(SettingsNRST *nrst);
};


struct SectorNRST
{
    // ���������� ��������� �� ���������� ���������, ���� ������� ������� � nullptr � ��������� ������
    SettingsNRST *GetSaved() const;

    // ���������� ��������� �� �����, ������������� �� ������ �������
    Packet *CreatePacket() const;

    // ���������� ��������� �� ��������� ���������� � ������� �����
    Packet *LastPacket() const;

    // ������� �������� � ������ ��������� � �������
    bool SaveSettings(SettingsNRST *) const;

    const Sector &sector;
};


static const SectorNRST sector1 = { HAL_ROM::sectors[Sector::_03_NRST_1] };
static const SectorNRST sector2 = { HAL_ROM::sectors[Sector::_04_NRST_2] };


void ROM::NRST::Save(SettingsNRST *nrst)
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


bool SectorNRST::SaveSettings(SettingsNRST *nrst) const
{
    Packet *last = LastPacket();

    if (last == nullptr)
    {
        last = (Packet *)sector.address;

        return last->SaveSettings(nrst);
    }

    return last->Next()->SaveSettings(nrst);
}


Packet *SectorNRST::LastPacket() const
{
    Packet *packet = CreatePacket();

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


bool Packet::SaveSettings(SettingsNRST *nrst)
{
    const Sector &sector = Sector::Get(Begin());

    if ((Begin() < sector.address) ||   // ��������� �� ����� �� ������ �������
        !IsEmpty()                 ||   // ��������� �� ��, ����� �� ����� ������ ������ �� ���� ��������
        (sizeof(SettingsNRST) + Begin() >= sector.End()))   // ��������� �� ����� �� ����� �������
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
