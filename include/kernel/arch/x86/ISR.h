#include <kernel/arch/x86/registers.h>
typedef void (*isr_t)(registers_t*);
extern void x86_ISR_RegisterHandler(uint8_t n, isr_t handler);