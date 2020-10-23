#pragma once

#define __sizeof_ptr 4
#define __FPU_USED 0

#include <stdint.h>

#ifndef __IO
#define __IO
#endif

void NVIC_SystemReset();
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
inline void __disable_irq() {};
inline void __set_MSP(uint32_t) {};
inline void __enable_irq() {};


typedef struct
{
    uint32_t VTOR;
} _SCB;


extern _SCB *SCB;
