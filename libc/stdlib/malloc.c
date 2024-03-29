#include <kernel/mem.h>
#include <kernel/panic.h>
#include <kernel/registers.h>
#include <string.h>
void *malloc(size_t size) {
	return PMM_AllocBytes(size);
}
/*
void *realloc(void *ptr, size_t size) {
	void *ptr2 = PMM_Alloc(NUM_PAGES(size));
	// copy over the data
	memcpy(ptr2, ptr, size);
	#ifdef MEM_DEBUG
		// zero out the rest of the memory.
		memset(ptr2 + size, 0, size);
	#endif

	// free the initial memory
	PMM_Free(ptr);

	// return the new memory.
	return ptr2;
}
*/
