#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "log.h"

static const char *level_names[] = { "fatal", "error", "warning", "info", "debug" };

void mushroom_log(int level, const char *file, int line, const char *format, ...)
{
	struct timespec tms;
	timespec_get(&tms, TIME_UTC);

	printf("[%li.%li] %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", tms.tv_sec, tms.tv_nsec / 10000000,
	       "\x1b[94m", level_names[level], file, line);

	va_list arglist;
	va_start(arglist, format);
	vprintf(format, arglist);
	va_end(arglist);

	printf("\n");
}