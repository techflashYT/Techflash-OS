#include <stdio.h>
#include <stdint.h>
uint64_t putcharWrapper(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	(void)arg2;
	(void)arg3;
	return (uint64_t)putchar(((int)arg1));
}