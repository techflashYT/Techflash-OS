#include <stdint.h>
void lidt(void* base, uint16_t size) {   // This function works in 32 and 64bit mode
	struct {
		uint16_t length;
		void*    base;
	} __attribute__((packed)) IDTR = { size, base };
 
	asm (
		"lidt %0"
		: : "m"(IDTR)
	);  // let the compiler choose an addressing mode
}
