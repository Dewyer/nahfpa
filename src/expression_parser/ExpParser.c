//
// Created by barna on 10/09/2020.
//

#include "ExpParser.h"
#include <stdio.h>
#include <logger/logger.h>
#include <utils/cassert.h>
#include <utils/symbols_helper.h>
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

TokenSlice *ExpParser_get_command_argument_slice(ExpParser *self, const DString *command, bool required, size_t start_i,
												 size_t max_i)
{
	const bool didnt_reached_end = start_i < max_i;
	if (required)
		cassert(self->logger, didnt_reached_end, "%s needs at least one argument but expression end reached.",
				DString_to_CString(command));
	else if (!didnt_reached_end)
		return NULL;

	const DString *starting_token = List_get(self->tokens, start_i);

	const bool starts_with_brace = DString_eq_CString(starting_token, "{");
	if (required)
		cassert(self->logger, starts_with_brace, "%s needs at least one argument.", DString_to_CString(command));
	else if (!starts_with_brace)
		return NULL;

	TokenSlice *arg_slice = TokenSlice_new(start_i, 0);
	bool found_ending_brace = false;
	int bracer_count = 0;

	for (size_t ii = start_i + 1; ii <= max_i; ++ii) {
		const DString *at_token = List_get(self->tokens, ii);
		if (DString_eq_CString(at_token, "{"))
			bracer_count++;
		else if (DString_eq_CString(at_token, "}")) {
			if (bracer_count == 0) {
				found_ending_brace = true;
				arg_slice->end = ii;
			} else
				bracer_count--;
		}
	}

	cassert(self->logger, found_ending_brace, "%s starting at token %u, has an argument without an ending bracket.",
			DString_to_CString(command), start_i);

	return arg_slice;
}

bool token_is_a_command(DString *token)
{
	return DString_eq_CString(token, "\\frac") ||
		   DString_eq_CString(token, "\\sqrt") ||
		   DString_eq_CString(token, "\\sum") ||
		   DString_eq_CString(token, "\\prod");
}

bool token_is_sub_or_superscript(DString *token)
{
	return DString_eq_CString(token, "_") ||
		   DString_eq_CString(token, "^");
}

bool token_might_be_symbol(DString *token)
{
	return DString_starts_with(token, "\\");
}

ExpNode *ExpParser_parse_node_list(ExpParser *self, TokenSlice *slice);

TokenSlice *
ExpParser_parse_command_args(ExpParser *self, ExpNode *command, int required_args, int max_args, size_t args_start,
							 size_t max_i)
{
	TokenSlice *arg1_slice = ExpParser_get_command_argument_slice(self, command->value, required_args >= 1, args_start,
																  max_i);
	TokenSlice *arg2_slice = NULL;
	if (arg1_slice && max_args >= 2)
		arg2_slice = ExpParser_get_command_argument_slice(self, command->value, required_args >= 2, arg1_slice->end + 1,
														  max_i);

	if (arg1_slice) {
		TokenSlice *real_sl1 = TokenSlice_shrink_clone(arg1_slice);
		command->arg1 = ExpParser_parse_node_list(self, real_sl1);
		command->arg1->parent = command;
	}
	if (arg2_slice) {
		TokenSlice *real_sl2 = TokenSlice_shrink_clone(arg2_slice);
		command->arg2 = ExpParser_parse_node_list(self, real_sl2);
		command->arg2->parent = command;
	}

	TokenSlice *final_slice;
	if (arg1_slice)
		final_slice = TokenSlice_new(arg1_slice->start, arg2_slice ? arg2_slice->end : arg1_slice->end);
	else
		final_slice = TokenSlice_new(args_start, args_start);

	TokenSlice_free(arg1_slice);
	TokenSlice_free(arg2_slice);
	return final_slice;
}

ExpNode *ExpParser_parse_command(ExpParser *self, DString *command, size_t command_start, size_t max_i)
{
	ExpNodeType type = Frac;
	int required_arg_c = 0;
	int max_arg_c = 0;

	if (DString_eq_CString(command, "\\frac")) {
		type = Frac;
		required_arg_c = 1;
		max_arg_c = 2;
	} else if (DString_eq_CString(command, "\\sqrt")) {
		type = Sqrt;
		required_arg_c = 1;
		max_arg_c = 1;
	} else if (DString_eq_CString(command, "\\sum")) {
		type = Sum;
		required_arg_c = 0;
		max_arg_c = 2;
	} else if (DString_eq_CString(command, "\\prod")) {
		type = Prod;
		required_arg_c = 0;
		max_arg_c = 2;
	}

	ExpNode *cmd_node = ExpNode_new(type);
	cmd_node->value = DString_clone(command);
	TokenSlice *final_slc = ExpParser_parse_command_args(self, cmd_node, required_arg_c, max_arg_c, command_start + 1,
														 max_i);
	cmd_node->origin = TokenSlice_new(command_start, final_slc->end);

	TokenSlice_free(final_slc);
	return cmd_node;
}

ExpNode *ExpParser_parse_from_string(DString *str, size_t ii)
{
	ExpNode *node = ExpNode_new(Literal);
	node->value = DString_clone(str);
	node->origin = TokenSlice_new(ii, ii);

	return node;
}

ExpNode *ExpParser_parse_sub_or_superscript(DString *script_token, size_t script_i)
{
	ExpNode *node = ExpNode_new(DString_eq_CString(script_token, "^") ? SuperScript : SubScript);
	node->value = DString_clone(script_token);
	node->origin = TokenSlice_new(script_i, script_i);

	return node;
}

ExpNode *ExpParser_parse_symbol(ExpParser *self, DString *sym, size_t sym_i)
{
	bool found_symbol = false;
	for (int ii = 0; ii < SUPPORTED_SYMBOL_COUNT; ++ii) {
		if (DString_eq_CString(sym, SUPPORTED_SYMBOLS[ii].command))
			found_symbol = true;
	}

	cassert(self->logger, found_symbol, "%s is not a supported command", DString_to_CString(sym));
	ExpNode *node = ExpParser_parse_from_string(sym, sym_i);
	node->type = Symbol;

	return node;
}

ExpNode *ExpParser_parse_inline_node_list(ExpParser *self, size_t start_i, size_t max_i)
{
	DString *cmd = DString_from_CString("Expression list");
	TokenSlice *slice = ExpParser_get_command_argument_slice(self, cmd, true, start_i, max_i);
	TokenSlice *real_slice = TokenSlice_shrink_clone(slice);

	ExpNode *node = ExpParser_parse_node_list(self, real_slice);
	if (node->origin)
		TokenSlice_free(node->origin);
	node->origin = slice;

	DString_free(cmd);
	return node;
}

void ExpParser_transform_node_list_for_scripts(ExpParser *self, ExpNode *node_list)
{
	cassert(self->logger, !!node_list->node_list, "Only node lists can be script transformed.");
	ListG(ExpNode*) *new_node_list = List_new();

	for (size_t ii = 0; ii < node_list->node_list->item_count; ++ii) {
		bool skip = false;
		ExpNode *at_node = List_get(node_list->node_list, ii);
		if ( ii < node_list->node_list->item_count-1)
		{
			ExpNode *next_node = List_get(node_list->node_list, ii+1);
			if (next_node->type == SubScript || next_node->type == SuperScript) {
				skip = true;
				next_node->arg1 = at_node;
				next_node->origin->start = at_node->origin->start;
			}
		}
		if (ii != 0)
		{
			ExpNode *last_node = List_get(node_list->node_list, ii-1);
			if (last_node->type == SubScript || last_node->type == SuperScript) {
				skip = true;
				last_node->arg2 = at_node;
				last_node->origin->end = at_node->origin->end;
			}
		}
		if (!skip)
			List_push(new_node_list, at_node);
	}

	List_free(node_list->node_list);
	node_list->node_list = new_node_list;
}

ExpNode *ExpParser_parse_node_list(ExpParser *self, TokenSlice *slice)
{
	ExpNode *node = ExpNode_new(NodeList);
	node->node_list = List_new();
	node->origin = slice;

	for (size_t at_token_i = slice->start; at_token_i <= slice->end; ++at_token_i) {
		DString *at_token = List_get(self->tokens, at_token_i);
		ExpNode *at_node;

		if (token_is_a_command(at_token)) {
			at_node = ExpParser_parse_command(self, at_token, at_token_i, slice->end);
			at_token_i = at_node->origin->end;
		} else if (token_is_sub_or_superscript(at_token)) {
			at_node = ExpParser_parse_sub_or_superscript(at_token, at_token_i);
		} else if (token_might_be_symbol(at_token)) {
			at_node = ExpParser_parse_symbol(self, at_token, at_token_i);
		} else if (DString_eq_CString(at_token, "{")) {
			at_node = ExpParser_parse_inline_node_list(self, at_token_i, slice->end);
			at_token_i = at_node->origin->end;
		} else {
			at_node = ExpParser_parse_from_string(at_token, at_token_i);
			at_node->type = Literal;
		}

		at_node->parent = node;
		List_push(node->node_list, at_node);
	}

	ExpParser_transform_node_list_for_scripts(self, node);
	return node;
}

ExpNode *ExpParser_do_parse(ExpParser *self)
{
	Logger_log(self->logger, LogInfo, "STEP 2. : Parse");

	TokenSlice *main_slice = TokenSlice_new(0, self->tokens->item_count - 1);
	ExpNode *root = ExpParser_parse_node_list(self, main_slice);

	Logger_log(self->logger, LogInfo, "=========== PARSED EXP ROOT : ==============");
	ExpNode_log(root, self->logger);
	Logger_log(self->logger, LogInfo, "=============================================");

	Logger_log(self->logger, LogInfo, "STEP 2. Parsing FINISHED!");

	return root;
}

void ExpParser_tokenize(ExpParser *self)
{
	Logger_log(self->logger, LogInfo, "STEP 1. : Tokenizer");
	Logger_log(self->logger, LogInfo, "INPUT: %s", DString_to_CString(self->raw_txt));

	ExpTokenizer *tokenizer = ExpTokenizer_new(DString_clone(self->raw_txt));
	ListG(DString*) *tokens = ExpTokenizer_tokenize(tokenizer);

	Logger_log(self->logger, LogInfo, "Tokenization successfull");

	for (int ii = 0; (size_t) ii < tokens->item_count; ii++) {
		DString *item = List_get(tokens, ii);
		Logger_log(self->logger, LogInfo, "\t[%d]Token:'%s'", ii, DString_to_CString(item));
	}
	Logger_log(self->logger, LogInfo, "STEP 1. Tokenization FINISHED!");
	self->tokens = tokens;

	ExpTokenizer_free(tokenizer);
}

ExpNode *ExpParser_parse(ExpParser *self)
{
	ExpParser_tokenize(self);
	return ExpParser_do_parse(self);
}

void ExpParser_free(ExpParser *self)
{
	DString_free(self->raw_txt);
	List_free_2D(self->tokens, (void (*)(void *)) DString_free);

	free(self);
}
