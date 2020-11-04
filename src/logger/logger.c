//
// Created by barna on 02/11/2020.
//
#include <stdbool.h>
#include "logger.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "dbgalloc/m.h"

struct Logger
{
	LogLevel min_level;
	FILE *out_file;
	bool should_free_out_file;
	bool use_color;
};

Logger *Logger_new(LogLevel min_level,char *out_path)
{
	Logger *self = (Logger *) malloc(sizeof(*self));

	self->min_level = min_level;
	if (out_path) {
		self->out_file = fopen(out_path, "w");
		self->use_color = false;
		self->should_free_out_file = true;
	}
	else {
		self->out_file = stdout;
		self->use_color = true;
		self->should_free_out_file = false;
	}

	return self;
}

void Logger_print_date(Logger *self)
{
	time_t time_now_seconds;
	time(&time_now_seconds);
	struct tm* time_now =localtime(&time_now_seconds);
	fprintf(self->out_file,"%d-%02d-%02dT%02d:%02d:%02d.000Z", time_now->tm_year+1900, time_now->tm_mon, time_now->tm_mday, time_now->tm_hour, time_now->tm_min, time_now->tm_sec); //2020-11-02T22:37:12.736Z
}

char* LogLevel_to_string(const LogLevel level)
{
	switch (level) {
		case LogInfo:
			return "INFO";
		case LogWarning:
			return "WARN";
		case LogError:
			return "ERR";
		default:
			return "OTHE";
	}
}

void Logger_print_head(Logger* self, LogLevel level)
{
	char *level_name = LogLevel_to_string(level);

	if (self->use_color)
	{
		switch (level) {
			case LogInfo:
				fprintf(self->out_file, "\033[1;36m");
				break;
			case LogWarning:
				fprintf(self->out_file, "\033[1;33m");
				break;
			case LogError:
				fprintf(self->out_file, "\033[1;31m");
				break;
			default:
				break;
		}
	}

	Logger_print_date(self);
	fprintf(self->out_file, "-%s: ", level_name);

	if (self->use_color)
		fprintf(self->out_file, "\033[0m");
}

void Logger_log(Logger *self, LogLevel level, char *format, ...)
{
	if (level < self->min_level)
		return;

	va_list ap;
	va_start(ap, format);

	Logger_print_head(self, level);
	vfprintf(self->out_file, format, ap);
	fprintf(self->out_file,"\n");

	va_end(ap);
}

void Logger_free(Logger* self)
{
	free(self);
}
