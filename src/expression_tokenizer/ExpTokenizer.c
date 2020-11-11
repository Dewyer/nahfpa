//
// Created by barna on 20/09/2020.
//

#include <stdbool.h>
#include "ExpTokenizer.h"
#include "data_structures/common.h"
#include "debugmalloc.h"

struct ExpTokenizer
{
	DString *raw_txt;
};

ExpTokenizer *ExpTokenizer_new(DString *raw_txt)
{
	ExpTokenizer *exp = (ExpTokenizer *) malloc(sizeof(exp));
	exp->raw_txt = raw_txt;

	return exp;
}

bool is_char_whitespace(char cc)
{
	return cc == ' ' || cc == '\t' || cc == '\n' || cc == '\r';
}

bool is_single_operator(char cc)
{
	return cc == '+' || cc == '-' || cc == '/' || cc == '*' || cc == '<' || cc == '>' || cc == '=' || cc == '{' ||
		   cc == '}'
		   || cc == '(' || cc == ')' || cc == '[' || cc == ']' || cc == '^';
}

bool flush_tokenizer(List *tokens, DString *last_token_buffer)
{
	if (DString_len(last_token_buffer) > 0) {
		List_push(tokens, last_token_buffer);
		return true;
	}

	DString_free(last_token_buffer);
	return false;
}

ListG(DString*) *ExpTokenizer_union_escape_sequences(ListG(DString*) *tokens)
{
	ListG(DString*) *new_tokens = List_new();

	for (int token_i = 0; (size_t)token_i < tokens->item_count -1; ++token_i) {
		const DString *at_token = List_get(tokens, token_i);
		const DString *next_token = List_get(tokens, token_i);


	}

	List_free_2D(tokens, (void (*)(void *)) DString_free);
	return new_tokens;
}

ListG(DString*) *ExpTokenizer_tokenize(const ExpTokenizer *self)
{
	ListG(DString*) *tokens = List_new();
	const int exp_len = DString_len(self->raw_txt);
	const char *raw_cstring = DString_to_CString(self->raw_txt);
	DString *last_token_buffer = DString_from_CString("");

	bool should_flush_next = false;

	for (int chr_ind = 0; chr_ind < exp_len; chr_ind++) {
		const char chr_at = raw_cstring[chr_ind];
		bool add_chr = true;
		bool flush = should_flush_next;
		bool should_flush_now_set_now = false;

		if (is_char_whitespace(chr_at)) {
			add_chr = false;
			flush = true;
		} else if (is_single_operator(chr_at)) {
			flush = true;
			should_flush_next = true;
			should_flush_now_set_now = true;
		}

		if (flush) {
			flush_tokenizer(tokens, last_token_buffer);
			last_token_buffer = DString_from_CString("");

			if (!should_flush_now_set_now)
				should_flush_next = false;
		}
		if (add_chr) {
			DString_add_char(last_token_buffer, chr_at);
		}
	}

	bool did_flush_last_token = flush_tokenizer(tokens, last_token_buffer);
	if (!did_flush_last_token)
		DString_free(last_token_buffer);


	return tokens;
}

void ExpTokenizer_free(ExpTokenizer *self)
{
	DString_free(self->raw_txt);
	free(self);
}
