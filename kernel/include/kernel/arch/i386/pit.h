#ifndef ARCH_I386_PIT_H
#define ARCH_I386_PIT_H
#include <stdint.h>

extern double timeSinceBoot;

void sleeps(double seconds);
void sleep(uint64_t milliseconds);

extern struct pit pit;

#endif
