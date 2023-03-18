#include <kernel/hardware/parallel.h>
#include <kernel/bda.h>
#include <kernel/hardware/IO.h>
#include <kernel/hardware/PIT.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
MODULE("PARALLEL");
parallel_t parallel;
bool structInit = false;
uint16_t LPT1 = 0;
uint16_t LPT2 = 0;
uint16_t LPT3 = 0;
extern void padTo(char *src, uint8_t padding);
// NOTE: This will always take a minimum of 20ms, since we wait for the device to read.
bool parallelWrite(const uint16_t port, const uint8_t data) {
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
	uint8_t offset1 = 53;
	uint8_t offset2 = 68;
	uint8_t offset3 = 83;
	char *str = "I/O Ports according to the BIOS Data Area: Port 1: 0xAAA; Port 2: 0xAAA; Port 3: 0xAAA";
	char *buffer = malloc(8);
	padTo(utoa(LPT1, buffer, 16), 3);
	memcpy(str + offset1, buffer, 3);
	padTo(utoa(LPT2, buffer, 16), 3);
	memcpy(str + offset2, buffer, 3);
	padTo(utoa(LPT3, buffer, 16), 3);
	memcpy(str + offset3, buffer, 3);
	free(buffer);
	log(MODNAME, str, LOGLEVEL_DEBUG);
	return;
}