// 2021/06/20 18:38:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <stm32f429xx.h>


void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
