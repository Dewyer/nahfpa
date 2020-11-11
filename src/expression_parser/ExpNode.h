//
// Created by barna on 20/09/2020.
//

#ifndef NAHFPA_EXPNODE_H
#define NAHFPA_EXPNODE_H

#include "data_structures/DString.h"
#include "data_structures/List.h"

typedef enum ExpNodeType {
	SuperScript,
	SubScript,
	Frac,
	NodeList,
	Symbol,
	Sum,
	Mul,
	Sqrt

} ExpNodeType;

typedef struct ExpNode
{
	ExpNodeType type;
	struct ExpNode* arg1;
	struct ExpNode* arg2;
	ListG(struct ExpNode)* node_list;
	DString* symbol;

	struct ExpNode* parent;

} ExpNode;

ExpNode* ExpNode_new(ExpNodeType type);
ExpNode* ExpNode_from_symbol(DString* symbol_str);
void ExpNode_free(ExpNode* self);

#endif //NAHFPA_EXPNODE_H
