#include <stdint.h>
#include <kernel/log.h>
#include <string.h>
char elfMagic[] = {0x7f, 'E', 'L', 'F'};
uint8_t elfCheckIsValid(uint8_t *header, uint8_t arch) {
	// if (header[0] != 0x7f || header[1] != 'E' || header[2] != 'L' || header[3] != 'F') {
	if (memcmp(header, elfMagic, 4) != 0) {
		log("ELFLOAD", "File does not have ELF magic (0x7F, 'E', 'L', 'F').");
		log("ELFLOAD", "Either this file is not an executable at all (likely), or it's heavily corrupted.");
		return 1;
	}
	return 0;
}