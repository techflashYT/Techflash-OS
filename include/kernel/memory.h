#include <stddef.h>
#include <stdbool.h>

typedef struct {
	bool free;
	size_t size;
} memControlBlock;

extern void mallocInit(uint8_t *addr, uint_fast64_t size);


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

/*
 * Function:  PMM_AllocatePage 
 * ---------------------------
 * Allocates a page of physical memory.
 * 
 * Parameters:
 *      void
 *
 * Returns:
 *      pointer to the allocated page, or NULL if no free pages are available.
 *
 */
extern void *PMM_AllocatePage(void);

/*
 * Function:  PMM_FreePage 
 * -----------------------
 * Frees a page of physical memory.
 * 
 * Parameters:
 *      page: pointer to the page to be freed.
 *
 * Returns:
 *      void
 *
 */
extern void PMM_FreePage(void *page);

extern bool heapSetUp;
extern bool haveAllocated;
extern uint8_t *lastValidHeapAddress;
extern uint8_t *heapSpace;