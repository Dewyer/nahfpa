//
// Created by barna on 02/11/2020.
//

#ifndef NAHFPA_LOGGER_H
#define NAHFPA_LOGGER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum LogLevel
{
	LogInfo = 0, LogWarning = 1, LogError = 2
} LogLevel;

typedef struct Logger Logger;

Logger *Logger_new(LogLevel min_level, char *out_path, bool use_color);

void Logger_free(Logger *self);

void Logger_log(Logger *self, LogLevel level, char *format, ...);

void Logger_log_varg(Logger *self, LogLevel level, char *format, va_list va_list);

char *LogLevel_to_string(const LogLevel level);

#endif //NAHFPA_LOGGER_H
