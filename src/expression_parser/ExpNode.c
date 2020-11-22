//
// Created by barna on 20/09/2020.
//

#include <logger/logger.h>
#include "ExpNode.h"
#include "debugmalloc.h"

ExpNode *ExpNode_new(ExpNodeType type)
{
	ExpNode *exp_node = (ExpNode *) malloc(sizeof(*exp_node));
	exp_node->node_list = NULL;
	exp_node->arg1 = NULL;
	exp_node->arg2 = NULL;
	exp_node->arg3 = NULL;
	exp_node->value = NULL;
	exp_node->origin = NULL;
	exp_node->type = type;
	exp_node->parent = NULL;

	return exp_node;
}

char *get_tab_prefix(int lvl)
{
	char *pref = (char *) calloc(2 * lvl + 1, sizeof(*pref));
	for (int ii = 0; ii < lvl; ++ii) {
		pref[ii * 2] = '\t';
		pref[ii * 2 + 1] = '\t';
	}
	pref[2 * lvl] = '\0';

	return pref;
}

char *type_to_string(ExpNodeType type)
{
	switch (type) {
		case Frac:
			return "FRAC";
		case Sqrt:
			return "SQRT";
		case Sum:
			return "SUM";
		case Prod:
			return "PROD";
		case Index:
			return "INDEX";
		case NodeList:
			return "NODE_LIST";
		case Symbol:
			return "SYMBOL";
		case Literal:
			return "LITERAL";
		default:
			return "UNKNOWN";
	}
}

void ExpNode_log_with_lvl(const ExpNode *self, Logger *logger, int lvl)
{
	char *prefix = get_tab_prefix(lvl);
	Logger_log(logger, LogInfo, "%s|T:%s| v: %s -- [%u;%u]", prefix, type_to_string(self->type),
			   self->value ? DString_to_CString(self->value) : "", self->origin ? self->origin->start : 0,
			   self->origin ? self->origin->end : 0);

	if (self->arg1) {
		Logger_log(logger, LogInfo, "%s-ARG1:", prefix);
		ExpNode_log_with_lvl(self->arg1, logger, lvl + 1);
	}
	if (self->arg2) {
		Logger_log(logger, LogInfo, "%s-ARG2:", prefix);
		ExpNode_log_with_lvl(self->arg2, logger, lvl + 1);
	}
	if (self->arg3) {
		Logger_log(logger, LogInfo, "%s-ARG3:", prefix);
		ExpNode_log_with_lvl(self->arg3, logger, lvl + 1);
	}

	if (self->node_list) {
		Logger_log(logger, LogInfo, "%s-NODE LIST:", prefix);
		for (size_t ii = 0; ii < self->node_list->item_count; ++ii) {
			const ExpNode *at_node = List_get(self->node_list, ii);

			ExpNode_log_with_lvl(at_node, logger, lvl + 1);
		}
	}

	free(prefix);
}

void ExpNode_log(const ExpNode *self, Logger *logger)
{
	ExpNode_log_with_lvl(self, logger, 0);
}

void ExpNode_free(ExpNode *self)
{
	if (self->node_list)
		List_free_2D(self->node_list, (void (*)(void *)) ExpNode_free);

	if (self->value)
		DString_free(self->value);

	if (self->arg1)
		ExpNode_free(self->arg1);

	if (self->arg2)
		ExpNode_free(self->arg2);

	if (self->arg3)
		ExpNode_free(self->arg3);

	if (self->origin)
		TokenSlice_free(self->origin);

	free(self);
}

