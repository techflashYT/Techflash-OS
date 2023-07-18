#pragma once
#include <stdint.h>
#include <stddef.h>
extern void PIT_Init(uint32_t frequency);
extern void sleep(size_t ms);
extern size_t PIT_GetTicks();