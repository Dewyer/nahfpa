//
// Created by barna on 15/11/2020.
//

#ifndef NAHFPA_BOXNODE_H
#define NAHFPA_BOXNODE_H

#include <svg_file/SvgFile.h>
#include <expression_parser/ExpNode.h>
#include <utils/geomerty.h>

typedef struct BoxNode
{
	Vector offset;
	Vector global_pos;
	Size box;
    double centering_axis_y;

	struct BoxNode *arg1_box;
	struct BoxNode *arg2_box;
	struct BoxNode *arg3_box;
	ListG(struct BoxNode) *node_list_box;

	ExpNode *node;
} BoxNode;

BoxNode *BoxNode_new(ExpNode *node);

void BoxNode_calc_global_position(BoxNode *node, const BoxNode *parent);

void BoxNode_free(BoxNode *self);

#endif //NAHFPA_BOXNODE_H
