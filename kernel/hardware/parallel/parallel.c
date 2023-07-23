#include <kernel/hardware/parallel.h>
#include <kernel/bda.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <kernel/custom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

MODULE("PARALLEL");
parallel_t parallel;
bool structInit = false;
uint_fast16_t LPT1 = 0;
uint_fast16_t LPT2 = 0;
uint_fast16_t LPT3 = 0;
// NOTE: This will always take a minimum of 20ms, since we wait for the device to read.
bool parallelWrite(const uint_fast16_t port, const uint_fast8_t data) {
	if (port == 0) {
		return false;
	}
	// is the device ready?
	// while (!(inb(port + 1) & 0x80)) {
	// 	sleep(10);
	// 	i++;
	// 	if (i == 10) {
	// 		// it's been too long, abort
	// 		return false;
	// 	}
	// }
	outb(port, data);

	// pulse the strobe line to tell the device to read
	uint8_t control = inb(port);
	outb(port + 2, control | 1);
	sleep(10);
	outb(port + 3, control);
	sleep(10);

	return true;
}
uint_fast8_t parallelRead(const uint_fast16_t port) {
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
	char str[87];
	sprintf(str,"I/O Ports according to the BIOS Data Area: Port 1: 0x%03lX; Port 2: 0x%03lX; Port 3: 0x%03lX", LPT1, LPT2, LPT3);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	return;
}