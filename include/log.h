#pragma once

#include <stdarg.h>

#define mushroom_log_debug(...) mushroom_log(__FILE__, __LINE__, __VA_ARGS__)

void mushroom_log(const char *file, int line, const char *format, ...);