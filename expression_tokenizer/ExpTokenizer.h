//
// Created by barna on 20/09/2020.
//

#ifndef NAHFPA_EXPTOKENIZER_H
#define NAHFPA_EXPTOKENIZER_H

#include "../data_structures/common.h"

typedef struct ExpTokenizer ExpTokenizer;
ExpTokenizer* ExpTokenizer_new(DString* raw_txt);
List* ExpTokenizer_tokenize(const ExpTokenizer* self);
void ExpTokenizer_free(ExpTokenizer* self);

#endif //NAHFPA_EXPTOKENIZER_H
