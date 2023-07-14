#pragma once
#include <stdint.h>
extern void initPIT(uint32_t frequency);
extern void sleep(uint64_t ms);