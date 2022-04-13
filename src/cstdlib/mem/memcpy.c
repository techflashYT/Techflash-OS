#include "../util.h"
void *memcpy(void *dest, const void *src, size_t count) {
	__asm__ volatile ("cld; rep movsb" : "+c" (count), "+S" (src), "+D" (dest) :: "memory");
	return dest;
}
