#include <stdint.h>
// A struct describing an interrupt gate.
typedef struct {
	uint16_t baseLow;             // Bit 0-15 of the address to jump to when this interrupt fires.
	uint16_t sel;                 // Kernel segment selector.
	uint8_t  always0;             // This must always be zero.
	uint8_t  flags;               // More flags. See documentation.
	uint16_t baseMiddle;          // Bits 16-31 of the address to jump to.
	uint32_t baseHigh;			  // Bits 32-63 of the address to jump to.
	uint32_t reserved;			  // Reserved bits.
} __attribute__((packed)) idtEntry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
typedef struct {
	uint16_t limit;
	uint64_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed)) idtPtr_t;

void IDTInit();
extern void ISR0();
extern void ISR1();
extern void ISR2();
extern void ISR3();
extern void ISR4();
extern void ISR5();
extern void ISR6();
extern void ISR7();
extern void ISR8();
extern void ISR9();
extern void ISR10();
extern void ISR11();
extern void ISR12();
extern void ISR13();
extern void ISR14();
extern void ISR15();
extern void ISR16();
extern void ISR17();
extern void ISR18();
extern void ISR19();
extern void ISR20();
extern void ISR21();
extern void ISR22();
extern void ISR23();
extern void ISR24();
extern void ISR25();
extern void ISR26();
extern void ISR27();
extern void ISR28();
extern void ISR29();
extern void ISR30();
extern void ISR31();
