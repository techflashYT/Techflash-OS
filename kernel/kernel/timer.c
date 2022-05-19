#include <kernel/timer.h>
/*!
	@brief Return the current number of seconds since the system was booted (currently just returns 0 since I never implemented a timer).
	@return The current number of seconds since the system was booted.
*/
int __internal_timer_now_seconds() {
	return 0;
}
/*!
	@brief Return the current number of milliseconds since the system was booted (currently just returns 0 since I never implemented a timer).
	@return The current number of milliseconds since the system was booted.
*/
int __internal_timer_now_milliseconds() {
	return 0;
}

