#include <stdbool.h>
extern uint16_t SERIAL_PORT_COM1;
extern uint16_t SERIAL_PORT_COM2;
extern uint16_t SERIAL_PORT_COM3;
extern uint16_t SERIAL_PORT_COM4;
// these aren't in the BDA, so these address are the best we can get
#define SERIAL_PORT_COM5 0x5F8
#define SERIAL_PORT_COM6 0x4F8
#define SERIAL_PORT_COM7 0x5E8
#define SERIAL_PORT_COM8 0x4E8
#include <stdint.h>
typedef struct {
	bool         working;
	bool         echo;
	bool         (*init)         (const uint_fast64_t speed);
	uint_fast8_t (*readNext)     (const uint_fast16_t port);
	void         (*write)        (const uint_fast16_t port, const uint_fast8_t value);
	void         (*writeString)  (const uint_fast16_t port, const char *value);
	int          (*readBufEmpty) (const uint_fast16_t port);
	int          (*writeBufEmpty)(const uint_fast16_t port);
} serial_t;
extern serial_t serial;
