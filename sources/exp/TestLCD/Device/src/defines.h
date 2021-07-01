// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines_.h"

#ifdef WIN32
#define __attribute__(x)
#define __packed__
#define __STATIC_INLINE inline
#define __RBIT(VAL) 0
#define __CLZ(x) 0
#define __regControl
#define __asm(x)
#else
#define nullptr 0
#endif


#define NUM_VER "1.3"

#ifndef __IO
#define __IO volatile
#endif


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))

#define BIN_U8( bits ) _bitset(0##bits)

#define DISABLE_RU  "Откл"
#define DISABLE_EN  "Disable"
#define ENABLE_RU   "Вкл"
#define ENABLE_EN   "Enable"

#define EXIT_RU "Выход"
#define EXIT_EN "Exit"
#define EXIT_ILLUSTRATION_RU "Кнопка для выхода в предыдущее меню"
#define EXIT_ILLUSTRATION_EN "Button for return to previous menu"
