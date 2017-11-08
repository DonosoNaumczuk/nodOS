#include <time.h>

static unsigned long ticks = 0;

#define SECOND 0
#define MINUTE 1
#define HOUR 2
#define DAY 3
#define MONTH 4
#define YEAR 5

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

uint64_t timeHandler(int c){
	switch(c) {
		case SECOND:
			return _getSeconds();
		case MINUTE:
			return _getMinutes();
		case HOUR:
			return _getHour();
		case DAY:
			return _getDay();
		case MONTH:
			return _getMonth();
		case YEAR:
			return _getYear();
	}
}
