#pragma once

#include <stdarg.h>

enum mushroom_log_level {
	MUSHROOM_LOG_FATAL,
	MUSHROOM_LOG_ERROR,
	MUSHROOM_LOG_WARNING,
	MUSHROOM_LOG_INFO,
	MUSHROOM_LOG_DEBUG
};

#define mushroom_log_debug(...) mushroom_log(MUSHROOM_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

void mushroom_log(int level, const char *file, int line, const char *format, ...);