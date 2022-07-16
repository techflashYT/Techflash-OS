#define SERIAL_PORT_COM1 0x3F8
#define SERIAL_PORT_COM2 0x2F8
#define SERIAL_PORT_COM3 0x3E8
#define SERIAL_PORT_COM4 0x2E8
#define SERIAL_PORT_COM5 0x5F8
#define SERIAL_PORT_COM6 0x4F8
#define SERIAL_PORT_COM7 0x5E8
#define SERIAL_PORT_COM8 0x4E8
#include <stdint.h>
typedef struct {
	int (*init)();
	char (*readNext)(uint16_t port);
	void (*write)(uint16_t port, char value);
} __serial_t;
extern __serial_t serial;