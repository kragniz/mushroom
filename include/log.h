#pragma once

#include <stdarg.h>

enum mushroom_log_level {
	MUSHROOM_LOG_FATAL,
	MUSHROOM_LOG_ERROR,
	MUSHROOM_LOG_WARN,
	MUSHROOM_LOG_INFO,
	MUSHROOM_LOG_DEBUG
};

#define _prefix 7
#define _file __FILE__ + _prefix

#define mushroom_log_debug(...) mushroom_log(MUSHROOM_LOG_DEBUG, _file, __LINE__, __VA_ARGS__)
#define mushroom_log_info(...) mushroom_log(MUSHROOM_LOG_INFO, _file, __LINE__, __VA_ARGS__)
#define mushroom_log_warn(...) mushroom_log(MUSHROOM_LOG_WARN, _file, __LINE__, __VA_ARGS__)
#define mushroom_log_error(...) mushroom_log(MUSHROOM_LOG_ERROR, _file, __LINE__, __VA_ARGS__)
#define mushroom_log_fatal(...) _mushroom_log_fatal(_file, __LINE__, __VA_ARGS__)

void mushroom_log(int level, const char *file, int line, const char *format, ...);
void _mushroom_log_fatal(const char *file, int line, const char *format, ...);
