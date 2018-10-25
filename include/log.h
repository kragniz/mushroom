#pragma once

#include <stdarg.h>

enum mushroom_log_level {
	MUSHROOM_LOG_FATAL,
	MUSHROOM_LOG_ERROR,
	MUSHROOM_LOG_WARN,
	MUSHROOM_LOG_INFO,
	MUSHROOM_LOG_DEBUG
};

#define mushroom_log_debug(...) mushroom_log(MUSHROOM_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define mushroom_log_info(...) mushroom_log(MUSHROOM_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define mushroom_log_warn(...) mushroom_log(MUSHROOM_LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define mushroom_log_error(...) mushroom_log(MUSHROOM_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define mushroom_log_fatal(...) mushroom_log(MUSHROOM_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void mushroom_log(int level, const char *file, int line, const char *format, ...);