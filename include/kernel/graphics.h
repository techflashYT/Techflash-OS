#include <stdint.h>
extern uint32_t readPixel(const uint_fast16_t x, const uint_fast16_t y);
extern void putPixel(const uint_fast16_t x, const uint_fast16_t y, const uint32_t value);
typedef struct {
	char character;
	uint32_t color;
} colorChar_t;

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
	} vga;
	uint32_t warn;
	uint32_t error;
} colors_t;
extern colors_t colors;
