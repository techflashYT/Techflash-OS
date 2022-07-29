#include <stdint.h>
typedef struct {
	uint8_t percent;
	struct {
		void (*create)(uint8_t x, uint8_t y, uint8_t width);
		void (*update)(uint8_t percent);
	} progressBar;
} __boot_t;
extern __boot_t boot;
