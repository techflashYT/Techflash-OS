#include <kernel/arch/i386/io.h>
#include <stdint.h>
void pitActivateChannel(uint8_t channel, uint8_t frequency) {
	unsigned int divider = 1193180 / frequency;

	outb(0x43, channel == 0 ? 0x36 : 0xB6);
	outb(0x40 + channel, (uint8_t)(divider & 0xFF));
	outb(0x40 + channel, (uint8_t)((divider >> 8) & 0xFF));
}
