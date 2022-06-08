#include <stdint.h>
/*!
	\brief Write a byte to the specified port.
	\param port The port to write to.
	\param val The byte to write to the port.
	\return None.
*/
void outb(uint16_t port, uint8_t val) {
	asm volatile (
		"outb %0, %1"
		:: "a"(val),
		"Nd"(port)
	);
}
