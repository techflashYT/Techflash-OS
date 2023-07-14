#pragma once
#include <stdbool.h>
#include <stdint.h>
/*
* ====== GDT Stuff ======
*/
typedef struct {
	uint16_t limitLow;           // The lower 16 bits of the limit.
	uint16_t baseLow;            // Bits 0-15 of the base.
	uint8_t  baseMiddle;         // The next 8 bits of the base.
	uint8_t  access;             // Access flags, determine what ring this segment can be used in.
	uint8_t  granularity;
	uint8_t  baseHigh;           // The next 8 bits of the base.
} __attribute__((packed)) gdtEntry_t;
typedef struct {
	uint16_t limit;              // The upper 16 bits of all selector limits.
	uint64_t base;               // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdtPtr_t;

extern void GDTInit();

/*
* ====== IDT Stuff ======
*/

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

extern void IDTInit();

/*
* ====== IRQ Stuff ======
*/
extern void IRQSetMask(uint8_t IRQline, bool status);

/*
* ====== ISR Stuff ======
*/

typedef struct {
	uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t intNo, errCode;
	uint64_t rip, cs, rflags, userRsp, ss;
} registers_t;
typedef void (*isr_t)(registers_t*);
extern void registerInterruptHandler(uint8_t n, isr_t handler);

/*
* ====== SSE Stuff ======
*/
extern uint8_t SSEInit();

/*
* ====== A bunch of function declarations ======
*/

extern void IRQ0();  extern void IRQ1();  extern void IRQ2();  extern void IRQ3();
extern void IRQ4();  extern void IRQ5();  extern void IRQ6();  extern void IRQ7();
extern void IRQ8();  extern void IRQ9();  extern void IRQ10(); extern void IRQ11();
extern void IRQ12(); extern void IRQ13(); extern void IRQ14(); extern void IRQ15();
extern void IRQ16(); extern void IRQ17(); extern void IRQ18(); extern void IRQ19();
extern void IRQ20(); extern void IRQ21(); extern void IRQ22(); extern void IRQ23();
extern void IRQ24(); extern void IRQ25(); extern void IRQ26(); extern void IRQ27();
extern void IRQ28(); extern void IRQ29(); extern void IRQ30(); extern void IRQ31(); 

extern void ISR0();  extern void ISR1();  extern void ISR2();  extern void ISR3();
extern void ISR4();  extern void ISR5();  extern void ISR6();  extern void ISR7();
extern void ISR8();  extern void ISR9();  extern void ISR10(); extern void ISR11();
extern void ISR12(); extern void ISR13(); extern void ISR14(); extern void ISR15();
extern void ISR16(); extern void ISR17(); extern void ISR18(); extern void ISR19();
extern void ISR20(); extern void ISR21(); extern void ISR22(); extern void ISR23();
extern void ISR24(); extern void ISR25(); extern void ISR26(); extern void ISR27();
extern void ISR28(); extern void ISR29(); extern void ISR30(); extern void ISR31(); 