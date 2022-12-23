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
	bool working;
	bool (*init)         (const uint64_t speed);
	char (*readNext)     (const uint16_t port);
	void (*write)        (const uint16_t port, const uint8_t value);
	void (*writeString)  (const uint16_t port, const char *value);
	int  (*readBufEmpty) (const uint16_t port);
	int  (*writeBufEmpty)(const uint16_t port);
} serial_t;
extern serial_t serial;