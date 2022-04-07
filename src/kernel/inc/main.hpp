extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

    void kernelMain();
}
bool pciInit();
bool usbInit();
void setupRealIDT();
void kprintf(int_fast8_t x, uint_fast8_t y, uint_fast8_t color, const int_fast8_t* str, ...);
