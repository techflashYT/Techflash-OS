#pragma once


int __internal_timer_now_seconds();
int __internal_timer_now_milliseconds();

typedef struct {
	struct {
		int (*seconds)();
		int (*milliseconds)();
	} __attribute__ ((packed)) now;
} __attribute__ ((packed)) __internal_Timer_t;
__attribute__ ((unused)) static __internal_Timer_t timer = {
	.now.seconds = &__internal_timer_now_seconds,
	.now.milliseconds = &__internal_timer_now_milliseconds
};
