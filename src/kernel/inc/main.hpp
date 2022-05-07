extern "C" {
    void kernelMain();
}
#include "../../terminal.hpp"
bool pciInit();
bool usbInit();
void setupRealIDT();
