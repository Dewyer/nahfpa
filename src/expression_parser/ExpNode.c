//
// Created by barna on 20/09/2020.
//

#include "ExpNode.h"
#include "debugmalloc.h"

ExpNode* ExpNode_new(ExpNodeType type)
{
	ExpNode* exp_node = (ExpNode*) malloc(sizeof(*exp_node));
	exp_node->node_list = NULL;
	exp_node->arg1 = NULL;
	exp_node->arg2 = NULL;
	exp_node->symbol = NULL;
	exp_node->type = type;

	if (type == NodeList)
	{
		exp_node->node_list = List_new();
	}
	return exp_node;
}

ExpNode* ExpNode_from_symbol(DString* symbol_str)
{
	ExpNode* node = ExpNode_new(Symbol);
	node->symbol = symbol_str;

	return node;
}

void ExpNode_free(ExpNode* self)
{
	if (self->node_list != NULL)
		List_free(self->node_list);
}