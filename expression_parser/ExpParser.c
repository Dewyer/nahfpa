//
// Created by barna on 10/09/2020.
//

#include "ExpParser.h"
#include <stdlib.h>
#include "../data_structures/DString.h"

struct ExpParser
{
    DString* raw_txt;
};

ExpParser* ExpParser_new(DString* raw_txt)
{
    ExpParser* exp = malloc(sizeof(exp));
    exp->raw_txt = raw_txt;
    return exp;
}
void ExpParser_free(ExpParser* self)
{
    DString_free(self->raw_txt);
    free(self);
}
