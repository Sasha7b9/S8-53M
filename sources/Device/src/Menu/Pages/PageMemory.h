#pragma once


class Page;


class PageMemory
{
public:

    class Latest
    {
    public:
        static const Page *self;
    };

    class Internal
    {
    public:
        static const Page *self;
    };

    class SetMask
    {
    public:
        static const Page *self;
    };

    class SetName
    {
    public:
        static const Page *self;
    };

    class FileManager
    {
    public:
        static const Page *self;
    };

    static void *GetPointer();

    static const Page *self;
};


void ChangeC_Memory_NumPoints(bool active);
void OnPressMemoryExtFileManager();
void Memory_SaveSignalToFlashDrive();
extern const void *pMspFileManager;
