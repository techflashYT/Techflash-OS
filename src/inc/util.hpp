void halt() {
    for (;;) {
        asm("hlt");
    }
}

unsigned short int* vga_buffer = (unsigned short int*)0xB8000;


/* 
    Print function, prints a string to the screen by writing into VGA RAM.

    ARGUMENTS:
        char* string, the string to print
        short color, the color to print the string in, (0-15), default is 15 (white)
    
    RETURNS: Nothing.
*/
void print(const uint_fast8_t* str, const uint_fast8_t& color = 7) {
    for (uint_fast8_t i = 0; str[i] != '\0'; i++) {
        *vga_buffer++ = str[i] | (color << 8);
    }
}
