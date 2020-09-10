//
// Created by barna on 10/09/2020.
//

#ifndef NAHFPA_EXPPARSER_H
#define NAHFPA_EXPPARSER_H

#include "../data_structures/DString.h"

typedef struct ExpParser ExpParser;
ExpParser* ExpParser_new(DString* raw_txt);
void ExpParser_free(ExpParser* self);

#endif //NAHFPA_EXPPARSER_H
