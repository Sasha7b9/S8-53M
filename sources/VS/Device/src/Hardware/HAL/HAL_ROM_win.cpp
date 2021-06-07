// 2021/05/20 23:02:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Memory/Sector_.h"


#define CLEAR_FLASH_FLAGS


static char memory_ROM[2 * 1024 * 1024];


const Sector HAL_ROM::sectors[Sector::Count] =
{
    { Sector::_00_BOOT_1,     0x00000000U, _16K  },
    { Sector::_01_BOOT_2,     0x00004000U, _16K  },
    { Sector::_02_BOOT_3,     0x00008000U, _16K  },
    { Sector::_03,            0x0000C000U, _16K  },
    { Sector::_04,            0x00010000U, _64K  },
    { Sector::_05_FIRM_1,     0x00020000U, _128K },
    { Sector::_06_FIRM_2,     0x00040000U, _128K },
    { Sector::_07_FIRM_3,     0x00060000U, _128K },
    { Sector::_08,            0x00080000U, _128K },
    { Sector::_09,            0x000A0000U, _128K },
    { Sector::_10_SETTINGS_1, 0x000C0000U, _128K },
    { Sector::_11_SETTINGS_2, 0x000E0000U, _128K },
    { Sector::_12_NRST_1,     0x00100000U, _16K  },
    { Sector::_13_NRST_2,     0x00104000U, _16K  },
    { Sector::_14,            0x00108000U, _16K  },
    { Sector::_15,            0x0010C000U, _16K  },
    { Sector::_16,            0x00110000U, _64K  },
    { Sector::_17,            0x00120000U, _128K },
    { Sector::_18,            0x00140000U, _128K },
    { Sector::_19_DATA_1,     0x00160000U, _128K },
    { Sector::_20_DATA_2,     0x00180000U, _128K },
    { Sector::_21_DATA_3,     0x001A0000U, _128K },
    { Sector::_22_DATA_4,     0x001C0000U, _128K },
    { Sector::_23_DATA_5,     0x001E0000U, _128K }
};


void Sector::Erase() const
{
}
