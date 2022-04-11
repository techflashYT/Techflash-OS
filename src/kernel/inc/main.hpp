extern "C" {
    void kernelMain();
}
bool pciInit();
bool usbInit();
void setupRealIDT();
