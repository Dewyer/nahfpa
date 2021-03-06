//
// Created by barna on 20/09/2020.
//

#include <stdbool.h>
#include <utils/cassert.h>
#include "ExpTokenizer.h"
#include "data_structures/common.h"
#include "debugmalloc.h"

struct ExpTokenizer
{
	Logger *logger;
	DString *raw_txt;
	ListG(*size_t) *token_to_text_i;
	ListG(*DString) *tokens;
};

ExpTokenizer *ExpTokenizer_new(Logger *logger, DString *raw_txt)
{
	ExpTokenizer *exp = (ExpTokenizer *) malloc(sizeof(*exp));
	exp->logger = logger;
	exp->raw_txt = raw_txt;
	exp->token_to_text_i = List_new();
	exp->tokens = NULL;

	return exp;
}

void size_t_free(size_t *pp)
{
	free(pp);
}

size_t *size_t_new(size_t tt)
{
	size_t *new_p = malloc(sizeof(*new_p));
	*new_p = tt;

	return new_p;
}

bool is_char_whitespace(char cc)
{
	return cc == ' ' || cc == '\t' || cc == '\n' || cc == '\r';
}

bool is_single_operator(char cc)
{
	return cc == '+' || cc == '-' || cc == '/' || cc == '*' || cc == '<' || cc == '>' || cc == '=' || cc == '{' ||
		   cc == '}'
		   || cc == '(' || cc == ')' || cc == '[' || cc == ']' || cc == '^' || cc == '_';
}

bool ExpTokenizer_flush_tokenizer(ExpTokenizer *self, List *tokens, DString *last_token_buffer, size_t at_char_i)
{
	if (DString_len(last_token_buffer) > 0) {
		List_push(self->token_to_text_i, size_t_new(at_char_i));
		List_push(tokens, last_token_buffer);
		return true;
	}

	DString_free(last_token_buffer);
	return false;
}

bool is_token_escapable(const DString *token)
{
	return DString_eq_CString(token, "{") || DString_eq_CString(token, "}");
}

ListG(DString*) *ExpTokenizer_union_escape_sequences(const ListG(DString*) *tokens)
{
	ListG(DString*) *new_tokens = List_new();
	int last_token_concated = -1;

	for (int token_i = 0; (size_t) token_i < tokens->item_count - 1; ++token_i) {
		DString *at_token = List_get(tokens, token_i);
		const DString *next_token = List_get(tokens, token_i + 1);
		const bool at_backslash = DString_eq_CString(at_token, "\\");

		if (at_backslash && is_token_escapable(next_token)) {
			DString *new_token = DString_clone(at_token);
			DString_concat(new_token, next_token);

			List_push(new_tokens, new_token);
			token_i++;
			last_token_concated = token_i;
		} else {
			List_push(new_tokens, DString_clone(at_token));
			last_token_concated = -1;
		}
	}

	if (last_token_concated != (int) tokens->item_count - 1)
		List_push(new_tokens, DString_clone(List_get(tokens, tokens->item_count - 1)));

	return new_tokens;
}

ListG(DString*) *ExpTokenizer_tokenize(ExpTokenizer *self)
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
		} else if (chr_at == '\\') {
			flush = true;
			add_chr = true;
		}

		if (flush) {
			ExpTokenizer_flush_tokenizer(self, tokens, last_token_buffer, (size_t) chr_ind);
			last_token_buffer = DString_new();

			if (!should_flush_now_set_now)
				should_flush_next = false;
		}
		if (add_chr) {
			DString_add_char(last_token_buffer, chr_at);
		}
	}

	ExpTokenizer_flush_tokenizer(self, tokens, last_token_buffer, (size_t) exp_len - 1);

	ListG(DString*) *escaped_tokens = ExpTokenizer_union_escape_sequences(tokens);
	List_free_2D(tokens, (void (*)(void *)) DString_free);

	self->tokens = escaped_tokens;
	return escaped_tokens;
}

DString *ExpTokenizer_get_token_substring(ExpTokenizer *self, size_t start, size_t end)
{
	cassert(self->logger, start < end, "Token substring start over end");
	cassert(self->logger, end < self->tokens->item_count - 1, "Token substring end over token length");

	size_t *str_start_i = List_get(self->token_to_text_i, start);
	size_t str_end_i;
	if (end == self->tokens->item_count - 1)
		str_end_i = DString_len(self->raw_txt) - 1;
	else {
		size_t *end_ptr = List_get(self->token_to_text_i, end + 1);
		str_end_i = *end_ptr - 1;
	}

	return DString_substring(self->raw_txt, *str_start_i, str_end_i);
}

void ExpTokenizer_free(ExpTokenizer *self)
{
	DString_free(self->raw_txt);
	List_free_2D(self->token_to_text_i, (void (*)(void *)) size_t_free);
	free(self);
}

const DString *ExpTokenizer_get_raw_txt(const ExpTokenizer *self)
{
	return self->raw_txt;
}
