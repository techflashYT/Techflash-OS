#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/environment.h>
#include <kernel/font.h>
#include <kernel/hardware/serial.h>
void __kernTTY_scroll(const char *numLines) {
	printf("fb_width * 10 * 4: %llu\r\n", (uint64_t)(((uint64_t)bootboot.fb_width * 10) * 4));
	for (uint64_t i = 1; i != (bootboot.fb_width * 10) * 4; i++) {
		serial.writeString(SERIAL_PORT_COM1, "Scrolling left, iteration #");
		char *buffer = malloc(5);
		memset(buffer, '\0', 5);
		itoa(i, buffer, 10);
		serial.writeString(SERIAL_PORT_COM1, buffer);
		free(buffer);
		serial.writeString(SERIAL_PORT_COM1, "\r\n");
		((uint32_t *)&fb)[i - 1] = ((uint32_t *)&fb)[i];
	}

}