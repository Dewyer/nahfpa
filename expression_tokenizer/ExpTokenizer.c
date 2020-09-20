//
// Created by barna on 20/09/2020.
//

#include <stdlib.h>
#include <stdbool.h>
#include "ExpTokenizer.h"
#include "../data_structures/List.h"

struct ExpTokenizer
{
	DString *raw_txt;
};

ExpTokenizer *ExpTokenizer_new(DString *raw_txt)
{
	ExpTokenizer *exp = malloc(sizeof(exp));
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

void flush_tokenizer(List *tokens, DString *last_token_buffer)
{
	if (DString_len(last_token_buffer) > 0)
		List_push(tokens, last_token_buffer);
}

List *ExpTokenizer_tokenize(const ExpTokenizer *self)
{
	List *tokens = List_new(); //<DString>
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

	flush_tokenizer(tokens, last_token_buffer);

	return tokens;
}

void ExpTokenizer_free(ExpTokenizer *self)
{
	DString_free(self->raw_txt);
	free(self);
}