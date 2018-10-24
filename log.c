#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "log.h"

#define timestamp_buffer_size 32

void mushroom_log(const char *file, int line, const char *format, ...)
{
	struct timespec tms;
	timespec_get(&tms, TIME_UTC);

	printf("[%li.%li] %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", tms.tv_sec, tms.tv_nsec / 10000000,
	       "\x1b[94m", "debug", file, line);

	va_list arglist;
	va_start(arglist, format);
	vprintf(format, arglist);
	va_end(arglist);

	printf("\n");
}