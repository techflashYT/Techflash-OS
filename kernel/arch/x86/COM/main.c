#include <stdint.h>
#include <stdbool.h>
#include <kernel/arch/x86/IO.h>
static bool x86_COM_TransmitBufIsEmpty(uint16_t port) {
	return (inb(port + 5) & 0x20) == 0;
}
void x86_COM_Write(uint16_t port, uint8_t byte) {
	while (x86_COM_TransmitBufIsEmpty(port) == 0);
	outb(port, byte);
}
void x86_COM_WriteStr(uint16_t port, const char *str) {
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		x86_COM_Write(port, str[i]);
	}
}