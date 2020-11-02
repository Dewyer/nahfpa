//
// Created by barna on 02/11/2020.
//

#ifndef NAHFPA_LOGGER_H
#define NAHFPA_LOGGER_H

#include <stdio.h>

typedef enum LogLevel {LogInfo=0, LogWarning=1, LogError=2} LogLevel;

typedef struct Logger Logger;

Logger* Logger_new(LogLevel min_level, FILE *out);
void Logger_free(Logger* self);

void Logger_log(Logger* self, LogLevel level, char* format, ...);

#endif //NAHFPA_LOGGER_H
