#pragma once


class Page;


class PageMemory
{
public:

    class Latest
    {
    public:
        static void *GetPointer();
    };

    class Internal
    {
    public:
        static void *GetPointer();
    };

    class SetMask
    {
    public:
        static void *GetPointer();
    };

    class SetName
    {
    public:
        static void *GetPointer();
    };

    class FileManager
    {
    public:
        static void *GetPointer();
    };

    static void *GetPointer();

    static const Page *self;
};


void ChangeC_Memory_NumPoints(bool active);
void OnPressMemoryExtFileManager();
void Memory_SaveSignalToFlashDrive();
extern const void *pMspFileManager;
