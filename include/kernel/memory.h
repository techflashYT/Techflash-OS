#include <stddef.h>
#include <stdbool.h>

typedef struct {
	bool free;
	size_t size;
} memControlBlock;

void mallocInit(unsigned char* addr);

extern bool heapSetUp;
extern bool haveAllocated;
extern unsigned char *lastValidHeapAddress;
extern unsigned char *heapSpace;