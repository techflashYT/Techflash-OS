#include <stdint.h>
#include <stdbool.h>
extern uint_fast16_t LPT1; // LPT1 can be either 0x378 or 0x3BC depending on the BIOS.
extern uint_fast16_t LPT2;
extern uint_fast16_t LPT3;
typedef struct {
	void         (*init) ();
	bool         (*write)(const uint_fast16_t port, const uint_fast8_t data);
	uint_fast8_t (*read) (const uint_fast16_t port);
	bool         working;
	bool         enabled;
} parallel_t;
extern parallel_t parallel;