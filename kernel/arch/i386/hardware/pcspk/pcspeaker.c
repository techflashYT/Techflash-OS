#include <stdint.h>
#include <kernel/arch/i386/io.h>
void playSound(uint32_t nFrequence) {
	uint32_t div;
	uint8_t tmp;

	// Set the PIT to the desired frequency
	div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (div) );
	outb(0x42, (uint8_t) (div >> 8));

	// And play the sound using the PC speaker
	tmp = inb(0x61);
	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

void noSound() {
	uint8_t tmp = inb(0x61) & 0xFC;

	outb(0x61, tmp);
}
