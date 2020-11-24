//
// Created by barna on 20/09/2020.
//

#ifndef NAHFPA_EXPTOKENIZER_H
#define NAHFPA_EXPTOKENIZER_H

#include "data_structures/common.h"

typedef struct ExpTokenizer ExpTokenizer;

ExpTokenizer *ExpTokenizer_new(Logger *logger,DString *raw_txt);

List *ExpTokenizer_tokenize(ExpTokenizer *self);

DString *ExpTokenizer_get_token_substring(ExpTokenizer *self, size_t start, size_t end);

void ExpTokenizer_free(ExpTokenizer *self);

const DString *ExpTokenizer_get_raw_txt(const ExpTokenizer *self);

#endif //NAHFPA_EXPTOKENIZER_H
