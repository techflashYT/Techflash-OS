#include <stdint.h>
uint16_t readPixel(const uint16_t x, const uint16_t y);


typedef struct {
	struct {
		uint32_t black;
		uint32_t blue;
		uint32_t green;
		uint32_t cyan;
		uint32_t red;
		uint32_t purple;
		uint32_t brown;
		uint32_t lgray;
		uint32_t dgray;
		uint32_t lblue;
		uint32_t lgreen;
		uint32_t lcyan;
		uint32_t lred;
		uint32_t pink;
		uint32_t yellow;
		uint32_t white;
	} colors;
} __vga;
extern __vga vga;
