#include <kernel/elf.h>
extern uint_fast8_t elfCheckIsValid(uint8_t *header, uint_fast8_t arch);
extern elfStruct_t *elfLoad(uint8_t *file);
void elfInit() {
	elfLoader.isValid = elfCheckIsValid;
	elfLoader.load    = elfLoad;
	return;
}