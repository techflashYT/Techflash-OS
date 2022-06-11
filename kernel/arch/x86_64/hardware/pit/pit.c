#include <kernel/arch/i386/io.h>
const uint64_t __pitBaseFreq = 1193182;
double timeSinceBoot = 0;

uint16_t divisor = 65535;

void sleeps(double seconds) {
	double startTime = timeSinceBoot;
	while (timeSinceBoot < startTime + seconds) {
		__asm__("hlt");
	}
}

void sleep(uint64_t milliseconds) {
	sleeps((double)(milliseconds * 1000));
}

void setDivisor(uint16_t newDivisor) {
	if (newDivisor > 100) {
		newDivisor = 100; // We don't want it going that fast
	}
	divisor = newDivisor;
	outb(0x40, (uint8_t)(newDivisor & 0x00FF));
	ioWait();
	outb(0x40, (uint8_t)(newDivisor & 0xFF00) >> 8);
}

uint64_t getFrequency() {
	return __pitBaseFreq / divisor;
}

void setFrequency(uint64_t frequency) {
	setDivisor((uint16_t)(__pitBaseFreq / frequency));
}

void __pitTick() {
	timeSinceBoot += 1 / (double)getFrequency();
}
struct {
	void (*setDivisor)(uint16_t divisor);
	uint64_t (*getFrequency)(void);
	void (*setFrequency)(uint64_t freq);
} pit;
