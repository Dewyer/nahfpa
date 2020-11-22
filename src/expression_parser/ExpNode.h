//
// Created by barna on 20/09/2020.
//

#ifndef NAHFPA_EXPNODE_H
#define NAHFPA_EXPNODE_H

#include "data_structures/DString.h"
#include "data_structures/List.h"
#include "TokenSlice.h"
#include "logger/logger.h"

typedef enum ExpNodeType
{
	Index,
	NodeList,
	Symbol,
	Literal,
	Frac,
	Sqrt,
	Sum,
	Prod,
	Lim,
	Comment

} ExpNodeType;

typedef struct ExpNode
{
	ExpNodeType type;
	struct ExpNode *arg1;
	struct ExpNode *arg2;
	struct ExpNode *arg3;
	ListG(struct ExpNode) *node_list;
	DString *value;
	TokenSlice *origin;

	struct ExpNode *parent;

} ExpNode;

ExpNode *ExpNode_new(ExpNodeType type);

void ExpNode_log(const ExpNode *self, Logger *logger);

void ExpNode_log_with_lvl(const ExpNode *self, Logger *logger, int lvl);

void ExpNode_free(ExpNode *self);

#endif //NAHFPA_EXPNODE_H
