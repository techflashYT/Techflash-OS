#include <stdint.h>
#include <stdbool.h>
#include <kernel/arch/x86/IO.h>
static int x86_COM_TransmitBufIsEmpty(uint16_t port) {
	return (inb(port + 5) & 0x20);
}
void x86_COM_Write(uint16_t port, uint8_t byte) {
	while (x86_COM_TransmitBufIsEmpty(port) == 0);
	outb(port, byte);
}