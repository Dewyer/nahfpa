//
// Created by barna on 10/09/2020.
//

#include "ExpParser.h"
#include <stdlib.h>
#include <stdio.h>
#include "../data_structures/DString.h"
#include "../expression_tokenizer/ExpTokenizer.h"
#include "../data_structures/List.h"

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

void ExpParser_parse(ExpParser* self)
{
	printf("STEP 1. : Tokenizer\n");
	ExpTokenizer* tokenizer = ExpTokenizer_new(self->raw_txt);
	List* tokens = ExpTokenizer_tokenize(tokenizer);
	printf("== Tokenization successfull\n");
	for (int ii = 0 ; ii < tokens->item_count; ii++)
	{
		DString* item = List_get(tokens,ii);
		printf("token:'%s'\n",DString_to_CString(item));
	}
	printf("STEP 1. Tokenization OVER!\n");
	
}

void ExpParser_free(ExpParser* self)
{
    DString_free(self->raw_txt);
    free(self);
}
