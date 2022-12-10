#include <stdint.h>
#include <kernel/log.h>
extern uint8_t elfCheckIsValid(void *header, uint8_t arch) {
	/*remember that strcmp returns zero when two strings are equal*/
	
	if (memcmp(header, "\x7fELF", 4)) {
		log("ELFLOAD", "File does not have ELF magic (0x7F, 'E', 'L', 'F').");
		log("ELFLOAD", "Either this file is not an executable at all (likely), or it's heavily corrupted.");
		return 1;
	}
}