#include "../util.h"
void *memset(void *b, int val, size_t count) {
	__asm__ volatile ("cld; rep stosb" : "+c" (count), "+D" (b) : "a" (val) : "memory");
	return b;
}
