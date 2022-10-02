#include <stdlib.h>
#include <kernel/memory.h>
void *malloc(size_t size) {
	return __malloc(size);
}