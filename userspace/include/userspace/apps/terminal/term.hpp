#ifndef USERSPACE_APPS_TERMINAL_TERM_HPP
#define USERSPACE_APPS_TERMINAL_TERM_HPP

#include <stdint.h>
#include <userspace/apps/settings/set.h>

typedef struct {
	uint8_t num;
	char buffer[settings.terminal.bufferSize]; // For now this will be a constant set to 2000 (num of chars in VGA).  TODO: Add a way to change it later + malloc this so that we aren't storing 8K on the stack
} terminal_t;

class Terminal {
	private:
		terminal_t currentTerm;
	public:
		Terminal();
		~Terminal();
		uint8_t getCurrentNumRunning();
		uint8_t getTermId();
};
#endif
