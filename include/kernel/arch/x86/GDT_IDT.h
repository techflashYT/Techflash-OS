extern void x86_GDT_Init();
extern void x86_IDT_Init();

typedef struct {
	uint16_t baseLow;             // Bit 0-15 of the address to jump to when this interrupt fires.
	uint16_t sel;                 // Kernel segment selector.
	uint8_t  always0;             // This must always be zero.
	uint8_t  flags;               // More flags. See documentation.
	uint16_t baseMiddle;          // Bits 16-31 of the address to jump to.
	uint32_t baseHigh;			  // Bits 32-63 of the address to jump to.
	uint32_t reserved;			  // Reserved bits.
} __attribute__((packed)) x86_IDT_Entry64_t;
typedef struct {
	uint16_t a;
	void *b;
} __attribute__((packed)) x86_IDT_Descriptor_t;

typedef struct {
	uint16_t limitLow;           // The lower 16 bits of the limit.
	uint16_t baseLow;            // Bits 0-15 of the base.
	uint8_t  baseMiddle;         // The next 8 bits of the base.
	uint8_t  access;             // Access flags, determine what ring this segment can be used in.
	uint8_t  granularity;
	uint8_t  baseHigh;           // The next 8 bits of the base.
} __attribute__((packed)) x86_GDT_Entry64_t;
typedef struct {
	uint16_t limit;              // The upper 16 bits of all selector limits.
	uint64_t base;               // The address of the first gdt_entry_t struct.
} __attribute__((packed)) x86_GDT_Ptr64_t;