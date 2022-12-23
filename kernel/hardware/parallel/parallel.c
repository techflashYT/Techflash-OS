#include <kernel/hardware/parallel.h>
#include <kernel/bda.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <kernel/log.h>
#include <stdio.h>
bool structInit = false;
uint16_t LPT1 = 0;
uint16_t LPT2 = 0;
uint16_t LPT3 = 0;
bool parallelWrite(const uint16_t port, const uint8_t data) {
	if (port == 0) {
		return false;
	}
	// is the device ready?
	uint8_t i = 0;
	while (!(inb(port + 1) & 0x80)) {
		sleep(10);
		i++;
		if (i == 10) {
			// it's been too long, abort
			return false;
		}
	}
	outb(port, data);

	// pulse the strobe line to tell the device to read
	uint8_t control = inb(port);
	outb(port + 2, control | 1);
	sleep(10);
	outb(port + 3, control);
	i = 0;
	while (!(inb(port + 1) & 0x80)) {
		sleep(10);
		i++;
		if (i == 10) {
			// it's been too long, abort
			return false;
		}
	}

	return true;
}
uint8_t parallelRead(const uint16_t port) {
	if (port == 0) {
		return 0;
	}
	return inb(port);
}
void parallelInit() {
	if (!structInit) {
		structInit       = true;
		LPT1             = bda->parallelPortAddrs[0];
		LPT2             = bda->parallelPortAddrs[1];
		LPT3             = bda->parallelPortAddrs[2];
		parallel.init    = parallelInit;
		parallel.write   = parallelWrite;
		parallel.read    = parallelRead;
		parallel.working = true;
		parallel.enabled = false;
		return;
	}
	outl(LPT1 + 2, 0); // reset control port with null
	bool failed = false;
	if (!parallelWrite(LPT1, 'A')) {
		failed = true;
	}
	if (!parallelWrite(LPT1, 'B')) {
		failed = true;
	}
	if (!parallelWrite(LPT1, 'C')) {
		failed = true;
	}
	if (failed) {
		log("LPTPORT", "Your parallel ports don't seem to work, disabling parallel logging.", LOGLEVEL_WARN);
		parallel.working = false;
	}
	return;
}