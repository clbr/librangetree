#ifndef TIMING_H
#define TIMING_H

#include <sys/time.h>
#include <time.h>
#include "ranget.h"

class timer {

public:
	timer() {
		gettimeofday(&tick, NULL);
	}

	u32 t() {
		struct timeval now;
		gettimeofday(&now, NULL);

		u32 msec = (now.tv_sec - tick.tv_sec) * 1000;
		msec += (now.tv_usec - tick.tv_usec) / 1000;

		tick = now;
		return msec;
	}

	void r() {
		gettimeofday(&tick, NULL);
	}
private:
	struct timeval tick;
};

#endif
