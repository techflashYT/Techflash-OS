#include <kernel/arch/i386/io.h>
/*!
	\brief Wait a very small time (a couple microseconds) by sending out a zero to a useless IO port.
	\return None.
*/
static inline void ioWait() {
	outb(0x80, 0); // Send out a zero to port 0x80 (Motherboard hex display port, useless after post)
}
