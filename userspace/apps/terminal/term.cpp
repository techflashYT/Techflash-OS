#include <userspace/apps/terminal/term.hpp>
Terminal::Terminal() {
	/* Initialize the current terminal number */
	Terminal::currentTerm.num = Terminal::getCurrentNumRunning() + 1;
}
Terminal::~Terminal() {
	// Do nothing since there is currently nothing to clean up
	// TODO: Once we implement malloc, make sure to free() the terminal buffer here
	// free(Terminal::currentTerm.buffer);
}
