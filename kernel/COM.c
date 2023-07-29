#include <stdint.h>
#include <kernel/arch/x86/COM.h>
void COM_Write(uint8_t serPort, uint8_t byte) {
	#ifdef __x86_64__
	uint16_t *portNums = &SERIAL_PORT_COM1;
	x86_COM_Write(portNums[serPort], byte);
	#else
	// TODO: log("COM_Write: Unimplemented for this platform", WARNING);
	#endif
}
void COM_WriteStr(uint8_t serPort, const char *str) {
	while (*str != '\0') {
		COM_Write(serPort, *str);
		str++;
	}
}
void COM_LogWrapper(const char ch, const uint16_t x, const uint16_t y, const uint32_t fgColor, const uint32_t bgColor) {
	(void)x;
	(void)y;
	(void)fgColor;
	(void)bgColor;
	COM_Write(0, ch);
}