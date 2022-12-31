#include <stdint.h>
typedef struct {
	uint8_t percent;
	struct {
		void (*create)(uint8_t x, uint8_t y, uint8_t width);
		void (*update)();
		void (*fadeOut)();
	} progressBar;
} boot_t;
extern boot_t boot;
