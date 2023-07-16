#include <stddef.h>
#include <stdbool.h>

typedef struct {
	bool free;
	size_t size;
} memControlBlock;

/*
 * Function:  PMM_Init 
 * --------------------
 * Initializes the physical memory manager.
 * 
 * Parameters:
 *      void
 *
 * Returns:
 *      void
 * 
 */
extern void PMM_Init(void);
extern void *PMM_Alloc(size_t size);
extern void PMM_Free(void* p);
#define ALIGN_PAGE(x) ((x + 4095) / 4096)