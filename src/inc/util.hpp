void halt() {
	for (;;) {
		asm("hlt");
	}
}

unsigned short int* vga_buffer = (unsigned short int*)0xB8000;

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_BROWN 14
#define WHITE 15


#define VGA_HEIGHT 25
#define VGA_WIDTH 80

struct Terminal {
	size_t row = 0;
	size_t column = 0;
	size_t width = VGA_WIDTH;
	size_t height = VGA_HEIGHT;
	uint_fast8_t fgColor = LIGHT_GRAY;
	uint_fast8_t bgColor = BLACK;
} terminal;


/* 
	Print function, prints a string to the screen by writing into VGA RAM.

	ARGUMENTS:
		char* string, the string to print
		uint_fast8_t color, the color to print the string in, (0-15), default is 15 (white)
	
	RETURNS: Nothing.
*/
void print(const uint_fast8_t* str, uint_fast8_t x, uint_fast8_t y, uint_fast8_t color = terminal.fgColor) {
	for (uint_fast8_t i = 0; str[i] != '\0'; i++) {
		vga_buffer[(y * VGA_WIDTH) + x + i] = str[i] | (color << 8);
	}
}



