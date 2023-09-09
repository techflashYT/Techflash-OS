#include <kernel/mem.h>
void free(void *ptr) {
	PMM_Free(ptr);
}
