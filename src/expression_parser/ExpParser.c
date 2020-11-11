//
// Created by barna on 10/09/2020.
//

#include "ExpParser.h"
#include <stdio.h>
#include <logger/logger.h>
#include "../expression_tokenizer/ExpTokenizer.h"
#include "debugmalloc.h"
#include "ExpNode.h"

struct ExpParser
{
	Logger *logger;

	DString *raw_txt;
	ListG(DString*) *tokens;
};

ExpParser *ExpParser_new(Logger *logger, DString *raw_txt)
{
	ExpParser *exp = (ExpParser *) malloc(sizeof(*exp));
	exp->logger = logger;
	exp->raw_txt = raw_txt;
	return exp;
}

void ExpParser_do_parse(ExpParser *self)
{
	Logger_log(self->logger, LogInfo, "STEP 2. : Parse");
	Logger_log(self->logger, LogInfo, "%d", DString_len(self->raw_txt));

	//ExpNode *leaf = ExpNode_new(NodeList);

	for (int token_i = 0; (size_t)token_i < self->tokens->item_count; token_i++)
	{

	}

	Logger_log(self->logger, LogInfo, "STEP 2. Parsing FINISHED!");
}

void ExpParser_tokenize(ExpParser *self)
{
	Logger_log(self->logger, LogInfo, "STEP 1. : Tokenizer");

	ExpTokenizer *tokenizer = ExpTokenizer_new(DString_clone(self->raw_txt));
	ListG(DString*) *tokens = ExpTokenizer_tokenize(tokenizer);

	Logger_log(self->logger, LogInfo, "Tokenization successfull");

	for (int ii = 0; (size_t) ii < tokens->item_count; ii++) {
		DString *item = List_get(tokens, ii);
		Logger_log(self->logger, LogInfo, "\tToken:'%s'", DString_to_CString(item));
	}
	Logger_log(self->logger, LogInfo, "STEP 1. Tokenization FINISHED!");
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
