#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

static const char *level_names[] = { "fatal", "error", "warning", "info", "debug" };

static int log_level = MUSHROOM_LOG_DEBUG;

static void
mushroom_log_msg(int level, const char *file, int line, const char *format, va_list argp)
{
	struct timespec tms;
	timespec_get(&tms, TIME_UTC);

	printf("[%li.%li] %s%-5s\x1b[0m\t\x1b[90m%s:%d:\x1b[0m\t", tms.tv_sec,
	       tms.tv_nsec / 10000000, "\x1b[94m", level_names[level], file, line);

	vprintf(format, argp);

	printf("\n");
}

void mushroom_log(int level, const char *file, int line, const char *format, ...)
{
	if (level > log_level) {
		return;
	}

	va_list arglist;
	va_start(arglist, format);
	mushroom_log_msg(level, file, line, format, arglist);
	va_end(arglist);
}

void _mushroom_log_fatal(const char *file, int line, const char *format, ...)
{
	va_list arglist;
	va_start(arglist, format);
	mushroom_log_msg(MUSHROOM_LOG_FATAL, file, line, format, arglist);
	va_end(arglist);

	exit(1);
}