#pragma once


class Page;


class PageMemory
{
public:

    class Latest
    {
    public:
        static Item *GetPointer();
    };

    class Internal
    {
    public:
        static Item *GetPointer();
    };

    class SetMask
    {
    public:
        static Item *GetPointer();
    };

    class SetName
    {
    public:
        static Item *GetPointer();
    };

    class FileManager
    {
    public:
        static Item *GetPointer();
    };

    static void *GetPointer();

    static const Page *self;
};


void ChangeC_Memory_NumPoints(bool active);
void OnPressMemoryExtFileManager();
void Memory_SaveSignalToFlashDrive();
extern const void *pMspFileManager;
