#include <stdint.h>

/*!
	\brief Read a byte from the specified port.
	\param port The port to read from.
	\return The byte read from the port.
*/
static inline uint8_t inb(uint16_t port) {
	uint8_t retVal;
	asm volatile (
		"inb %1, %0"
		: "=a"(retVal)
		: "Nd"(port)
	);
	return retVal;
}

