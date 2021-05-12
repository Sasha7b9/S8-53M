// 2021/05/12 14:11:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcName, PageMemory::PageExternal::self,
    "Имя файла", "File name"
    ,
    "Задаёт режим наименования файлов при сохранении на внешний накопитель:\n"
    "\"По маске\" - файлы именуются автоматически по заранее введённой маске (след. пункт меню),\n"
    "\"Вручную\" - каждый раз имя файла нужно задавать вручную"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "По маске", "Mask",
    "Вручную", "Manually",
    set.memory.file_naming_mode, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcModeSave, PageMemory::PageExternal::self,
    "Сохранять как", "Save as"
    ,
    "Если выбран вариант \"Изображение\", сигнал будет сохранён в текущем каталоге в графическом файле с расширением "
    "BMP\n Если выбран вариант \"Текст\", сигнал будет сохранён в текущем каталоге в текстовом виде в файле с "
    "раширением TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension "
    "BMP\nIf you select \"Text\", the signal will be stored in the current directory as a text file with the extension "
    "TXT"
    ,
    "Изображение", "Image",
    "Текст", "Text",
    set.memory.mode_save_signal, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcModeBtnMemory, PageMemory::PageExternal::self,
    "Реж кн ПАМЯТЬ", "Mode btn MEMORY",
    "",
    "",
    "Меню", "Menu",
    "Сохранение", "Save",
    set.memory.mode_button_memory, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcAutoConnectFlash, PageMemory::PageExternal::self,
    "Автоподкл.", "AutoConnect",
    "Eсли \"Вкл\", при подключении внешнего накопителя происходит автоматический переход на страницу ПАМЯТЬ - Внешн ЗУ",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.memory.flash_auto_connect, nullptr, nullptr, nullptr
)


DEF_PAGE_6(pageExternal, PageMemory::self, NamePage::MemoryExt,
    "ВНЕШН ЗУ", "EXT STORAGE",
    "Работа с внешним запоминающим устройством.",
    "Work with external storage device.",
    *PageMemory::PageFileManager::self,
    mcName,
    *PageMemory::PageSetMask::self,
    mcModeSave,
    mcModeBtnMemory,
    mcAutoConnectFlash,
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageMemory::PageExternal::self = &pageExternal;


void PageMemory::PageExternal::OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    int8(*func[3])(int8 *, int8, int8) =
    {
        Math::CircleDecrease<int8>,
        Math::CircleDecrease<int8>,
        Math::CircleIncrease<int8>
    };

    Color::ResetFlash();

    if (set.memory.index_cur_symbol_name_mask > maxIndex)
    {
        set.memory.index_cur_symbol_name_mask = (int8)(maxIndex - 1);
    }

    func[Math::Sign(angle) + 1](&set.memory.index_cur_symbol_name_mask, 0, (int8)(maxIndex - 1));
    Sound::RegulatorSwitchRotate();
}
