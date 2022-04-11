#include <stdint.h>
uint32_t farpeekl(uint16_t sel, void* off);
void farpokeb(uint16_t sel, void* off, uint8_t v);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

