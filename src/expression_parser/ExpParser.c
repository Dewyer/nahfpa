//
// Created by barna on 10/09/2020.
//

#include "ExpParser.h"
#include <stdlib.h>
#include <stdio.h>
#include "../expression_tokenizer/ExpTokenizer.h"
#include "dbgalloc/m.h"

struct ExpParser
{
	DString *raw_txt;
	ListG(DString*) *tokens;
};

ExpParser *ExpParser_new(DString *raw_txt)
{
	ExpParser *exp = (ExpParser*) malloc(sizeof(exp));
	exp->raw_txt = raw_txt;
	return exp;
}

void ExpParser_do_parse(ExpParser *self)
{
	printf("STEP 2. : Parse\n");
	printf("%d", DString_len(self->raw_txt));
	printf("STEP 2. Parsing FINISHED!\n");
}

void ExpParser_tokenize(ExpParser *self)
{
	printf("STEP 1. : Tokenizer\n");
	ExpTokenizer *tokenizer = ExpTokenizer_new(self->raw_txt);
	ListG(DString*) *tokens = ExpTokenizer_tokenize(tokenizer);
	printf("== Tokenization successfull\n");
	for (int ii = 0; (size_t)ii < tokens->item_count; ii++) {
		DString *item = List_get(tokens, ii);
		printf("token:'%s'\n", DString_to_CString(item));
	}
	printf("STEP 1. Tokenization FINISHED!\n");
	self->tokens = tokens;

	ExpTokenizer_free(tokenizer);
}

void ExpParser_parse(ExpParser *self)
{
	ExpParser_tokenize(self);
	ExpParser_do_parse(self);
}


void ExpParser_free(ExpParser *self)
{
	DString_free(self->raw_txt);
	List_free_2D(self->tokens, (void (*)(void *)) DString_free);

	free(self);
}
