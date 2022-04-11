#include "../util.hpp"
void *memcpy(void *dest, const void *src, size_t count) {
	asm volatile ("cld; rep movsb" : "+c" (count), "+S" (src), "+D" (dest) :: "memory");
	return dest;
}
