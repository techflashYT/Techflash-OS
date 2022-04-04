#include "io.hpp"
static inline void halt() {
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
	struct Cusor {
		void enable(uint_fast8_t start, uint_fast8_t end) {
			outb(0x3D4, 0x0A);
			outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

			outb(0x3D4, 0x0B);
			outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
		}
		void disable() {
			outb(0x3D4, 0x0A);
			outb(0x3D5, 0x20);
		}
		struct Position {
			uint_fast8_t x = 0;
			uint_fast8_t y = 0;
			uint_fast16_t __pos = 0;
			void change(uint_fast8_t posx, uint_fast8_t posy) {
				__pos = posy * VGA_WIDTH + posx;
				x = posx;
				y = posy;
				outb(0x3D4, 0x0F);
				outb(0x3D5, (uint8_t) (__pos & 0xFF));
				outb(0x3D4, 0x0E);
				outb(0x3D5, (uint8_t) ((__pos >> 8) & 0xFF));
			}
		} position;
	} cursor;
} terminal;

struct Keyboard {
	struct LEDs {
		bool num;
		bool caps;
		bool scroll;
	} leds;
	struct FunctionKeys {
		bool F1;
		bool F2;
		bool F3;
		bool F4;
		bool F5;
		bool F6;
		bool F7;
		bool F8;
		bool F9;
		bool F10;
		bool F11;
		bool F12;
	} funcKeys;
	struct NumpadKeys {
		bool bkslash;
		bool asterisk;
		bool minus;
		bool seven;
		bool eight;
		bool nine;
		bool plus;
		bool four;
		bool five;
		bool six;
		bool one;
		bool two;
		bool three;
		bool enter;
		bool zero;
		bool period;
	} numpad;
	struct ArrowKeys {
		bool up;
		bool down;
		bool left;
		bool right;
	} arrowKeys;
	struct SpecialKeys {
		bool printScr;
		bool pauseBreak;
		bool sysRq; // Same as pauseBreak
		bool insert;
		bool home;
		bool pageUp;
		bool deleteKey;
		bool end;
		bool pageDown;
		bool escape;
	} special;
	struct Modifiers {
		bool shift;
		bool ctrl;
		bool alt;
		bool fn;
		bool menu; // The key next to FN
	} modifiers;
	struct RegularKeys {
		bool tilde;
		bool one;
		bool two;
		bool three;
		bool four;
		bool five;
		bool six;
		bool seven;
		bool eight;
		bool nine;
		bool zero;
		bool minus;
		bool equals;
		bool backspace;
		bool tab;
		bool q;
		bool w;
		bool e;
		bool r;
		bool t;
		bool y;
		bool u;
		bool i;
		bool o;
		bool p;
		bool leftBracket;
		bool rightBracket;
		bool forwardSlash;
		bool a;
		bool s;
		bool d;
		bool f;
		bool g;
		bool h;
		bool j;
		bool k;
		bool l;
		bool semicolon;
		bool apostrophe;
		bool enter;
		bool z;
		bool x;
		bool c;
		bool v;
		bool b;
		bool n;
		bool m;
		bool comma;
		bool dot;
		bool backslash;
		bool leftWin;
		bool spacebar;
		bool rightWin;
	} keys;
} keyboard;


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



