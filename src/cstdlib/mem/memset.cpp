#include "../util.hpp"
void *memset(void *b, int val, size_t count) {
	asm volatile ("cld; rep stosb" : "+c" (count), "+D" (b) : "a" (val) : "memory");
	return b;
}
