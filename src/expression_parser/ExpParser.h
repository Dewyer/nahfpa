//
// Created by barna on 10/09/2020.
//

#ifndef NAHFPA_EXPPARSER_H
#define NAHFPA_EXPPARSER_H

#include <logger/logger.h>
#include <data_structures/DString.h>
#include "ExpNode.h"

typedef struct ExpParser ExpParser;

ExpParser *ExpParser_new(Logger *logger, DString *raw_txt);

ExpNode *ExpParser_parse(ExpParser *self);

void ExpParser_free(ExpParser *self);

#endif //NAHFPA_EXPPARSER_H
