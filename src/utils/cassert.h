//
// Created by barna on 03/11/2020.
//

#ifndef NAHFPA_CASSERT_H
#define NAHFPA_CASSERT_H

#include <logger/logger.h>

void cassert(Logger *log_inst, int exp, char* msg, ...);

#endif //NAHFPA_CASSERT_H
