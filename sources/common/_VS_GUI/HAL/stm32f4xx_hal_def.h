// 2021/06/20 18:31:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;


#define UNUSED(X) (void)X


typedef enum
{
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;
