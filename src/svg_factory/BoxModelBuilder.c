//
// Created by barna on 15/11/2020.
//

#include <utils/rendering_constants.h>
#include "BoxModelBuilder.h"
#include "BoxNode.h"
#include "debugmalloc.h"

struct BoxModelBuilder
{
	Logger *logger;
	ExpNode *exp_tree_root;
};

BoxModelBuilder *BoxModelBuilder_new(Logger *logger, ExpNode *exp_tree_root)
{
	BoxModelBuilder *self = (BoxModelBuilder *) malloc(sizeof(*self));
	self->logger = logger;
	self->exp_tree_root = exp_tree_root;

	return self;
}

void BoxModelBuilder_free(BoxModelBuilder *self)
{
	free(self);
}

BoxNode *BoxModelBuilder_build_box_for_node(ExpNode *exp_node);

void BoxModelBuilder_build_script_box(BoxNode *box_node, bool is_super)
{
	const Vector *delta = &SCRIPT_BOX_DELTA;

	if (box_node->node->arg1)
		box_node->arg1_box = BoxModelBuilder_build_box_for_node(box_node->node->arg1);

	if (box_node->node->arg2)
		box_node->arg2_box = BoxModelBuilder_build_box_for_node(box_node->node->arg2);

	if (box_node->arg1_box) {
		if (is_super)
			box_node->arg1_box->offset.y = box_node->arg2_box ? box_node->arg2_box->box.height + delta->y : 0;

		Size_add_s(&box_node->box, &box_node->arg1_box->box);
	}
	if (box_node->arg1_box && box_node->arg2_box) {
		box_node->arg2_box->offset.x = box_node->arg1_box->box.with + delta->x;

		Size_add_v(&box_node->box, delta);
	}
	if (box_node->arg2_box) {
		if (!is_super)
			box_node->arg2_box->offset.y = box_node->arg1_box ? box_node->arg1_box->box.height + delta->y : 0;

		Size_add_s(&box_node->box, &box_node->arg2_box->box);
	}
}

void BoxModelBuilder_build_node_list_box(BoxNode *box_node)
{
	box_node->node_list_box = List_new();
	const double gap = NODE_LIST_GAP;

	for (size_t ii = 0; ii < box_node->node->node_list->item_count; ++ii) {
		ExpNode *at_node = List_get(box_node->node->node_list, ii);
		BoxNode *at_box = BoxModelBuilder_build_box_for_node(at_node);

		if (box_node->box.height < at_box->box.height)
			box_node->box.height = at_box->box.height;

		at_box->offset.x = box_node->box.with;
		box_node->box.with += at_box->box.with + gap;

		List_push(box_node->node_list_box, at_box);
	}

	for (size_t ii = 0; ii < box_node->node_list_box->item_count; ++ii) {
		BoxNode *at_box = List_get(box_node->node_list_box, ii);

		Box_vertical_center(&at_box->offset, &at_box->box, &box_node->box);
	}

	box_node->box.with -= gap;
}

void BoxModelBuilder_build_frac_box(BoxNode *box_node)
{
	const double line_height = FRAC_LINE_HEIGHT;

	if (box_node->node->arg1) {
		box_node->arg1_box = BoxModelBuilder_build_box_for_node(box_node->node->arg1);
		box_node->box.height += box_node->arg1_box->box.height;
	}

	if (box_node->node->arg2) {
		box_node->arg2_box = BoxModelBuilder_build_box_for_node(box_node->node->arg2);
		box_node->box.height += box_node->arg2_box->box.height;
	}

	box_node->box.with = double_max(box_node->arg1_box ? box_node->arg1_box->box.with : 0,
									box_node->arg2_box ? box_node->arg2_box->box.with : 0);
	box_node->box.height += line_height;

	if (box_node->arg1_box) {
		Box_horizontal_center(&box_node->arg1_box->offset, &box_node->arg1_box->box, &box_node->box);
	}
	if (box_node->arg2_box) {
		Box_horizontal_center(&box_node->arg2_box->offset, &box_node->arg2_box->box, &box_node->box);
		box_node->arg2_box->offset.y = (box_node->arg1_box ? box_node->arg1_box->box.height : 0) + line_height;
	}
}

void BoxModelBuilder_build_box_for_sqrt(BoxNode *box_node)
{
	const Vector *delta = &SQRT_BOX_DELTA;

	box_node->arg1_box = BoxModelBuilder_build_box_for_node(box_node->node->arg1);
	Size_add_s(&box_node->box, &box_node->arg1_box->box);
	Size_add_v(&box_node->box, delta);
}

void BoxModelBuilder_build_prod_sum_box(BoxNode *box_node)
{
	BoxModelBuilder_build_frac_box(box_node);
	box_node->box.height -= FRAC_LINE_HEIGHT;
	box_node->box.height += SUM_PROD_SIZE.height;
	box_node->box.with = double_max(box_node->box.with, SUM_PROD_SIZE.with);
}

BoxNode *BoxModelBuilder_build_box_for_node(ExpNode *exp_node)
{
	BoxNode *box = BoxNode_new(exp_node);

	if (exp_node->type == Symbol) {
		box->box.with = TEXT_WIDTH;
		box->box.height = TEXT_HEIGHT + TEXT_CORRECTION;
	} else if (exp_node->type == Literal) {
		box->box.height = TEXT_HEIGHT + TEXT_CORRECTION;
		box->box.with = DString_len(exp_node->value) * TEXT_WIDTH;
	} else if (exp_node->type == SuperScript) {
		BoxModelBuilder_build_script_box(box, true);
	} else if (exp_node->type == SubScript) {
		BoxModelBuilder_build_script_box(box, false);
	} else if (exp_node->type == NodeList) {
		BoxModelBuilder_build_node_list_box(box);
	} else if (exp_node->type == Frac) {
		BoxModelBuilder_build_frac_box(box);
	} else if (exp_node->type == Sqrt) {
		BoxModelBuilder_build_box_for_sqrt(box);
	} else if (exp_node->type == Sum || exp_node->type == Prod) {
		BoxModelBuilder_build_prod_sum_box(box);
	}

	return box;
}

BoxNode *BoxModelBuilder_build(BoxModelBuilder *self)
{
	Logger_log(self->logger, LogInfo, "STEP 3. Building box model.");

	BoxNode *root = BoxModelBuilder_build_box_for_node(self->exp_tree_root);
	return root;
}
