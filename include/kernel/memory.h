#include <stddef.h>
#include <stdbool.h>

typedef struct {
	bool free;
	size_t size;
} memControlBlock;

void mallocInit(uint8_t *addr);

extern bool heapSetUp;
extern bool haveAllocated;
extern uint8_t *lastValidHeapAddress;
extern uint8_t *heapSpace;