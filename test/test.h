#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ranget.h"

static inline void fail(const char fmt[], ...) {

	va_list ap;
	va_start(ap, fmt);

	vfprintf(stderr, fmt, ap);

	va_end(ap);

	exit(1);
}

#endif
