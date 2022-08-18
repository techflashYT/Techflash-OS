#include <kernel/hardware/CPU/regs.h>
typedef void (*isr_t)(registers_t*);
void registerInterruptHandler(uint8_t n, isr_t handler);