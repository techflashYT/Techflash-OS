typedef struct {
	uint16_t limitLow;           // The lower 16 bits of the limit.
	uint16_t baseLow;            // The lower 16 bits of the base.
	uint16_t baseMiddle;         // The next 8 bits of the base.
	uint8_t  access;              // Access flags, determine what ring this segment can be used in.
	uint8_t  granularity;
	uint8_t  baseHigh;           // The last 8 bits of the base.
} __attribute__((packed)) gdtEntry_t;
typedef struct {
	uint16_t limit;               // The upper 16 bits of all selector limits.
	uint32_t base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdtPtr_t;

void GDTInit();