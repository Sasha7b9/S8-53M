// 2021/05/12 14:11:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Items/MenuItems.h"
#include "MEnu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcMemoryExtName, PageMemory::PageExternal::self,
    "��� �����", "File name"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "�� �����", "Mask",
    "�������", "Manually",
    set.memory.file_naming_mode, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcMemoryExtModeSave, PageMemory::PageExternal::self,
    "��������� ���", "Save as"
    ,
    "���� ������ ������� \"�����������\", ������ ����� �������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� �������� � ������� �������� � ��������� ���� � ����� � ���������� TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension BMP\n"
    "If you select \"Text\", the signal will be stored in the current directory as a text file with the extension TXT"
    ,
    "�����������", "Image",
    "�����", "Text",
    set.memory.mode_save_signal, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcMemoryExtModeBtnMemory, PageMemory::PageExternal::self,
    "��� �� ������", "Mode btn MEMORY",
    "",
    "",
    "����", "Menu",
    "����������", "Save",
    set.memory.mode_button_memory, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcMemoryExtAutoConnectFlash, PageMemory::PageExternal::self,
    "���������.", "AutoConnect",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.memory.flash_auto_connect, nullptr, nullptr, nullptr
)


DEF_PAGE_6(pageExternal, PageMemory::self, NamePage::MemoryExt,
    "����� ��", "EXT STORAGE",
    "������ � ������� ������������ �����������.",
    "Work with external storage device.",
    *PageMemory::PageFileManager::self,
    mcMemoryExtName,
    *PageMemory::PageSetMask::self,
    mcMemoryExtModeSave,
    mcMemoryExtModeBtnMemory,
    mcMemoryExtAutoConnectFlash,
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageMemory::PageExternal::self = &pageExternal;
