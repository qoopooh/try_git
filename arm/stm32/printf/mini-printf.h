/*
 * The Minimal snprintf() implementation
 * Copyright (c) 2013 Michal Ludvig <michal@logix.cz>
 */


#ifndef MINI_PRINTF_H
#define MINI_PRINTF_H

#include <stdarg.h>

extern int mini_vsnprintf(char* buffer, unsigned int buffer_len, char *fmt, va_list va);
extern int mini_snprintf(char* buffer, unsigned int buffer_len, char *fmt, ...);

#define vsnprintf mini_vsnprintf
#define snprintf mini_snprintf

#endif
