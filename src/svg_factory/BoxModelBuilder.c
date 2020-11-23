//
// Created by barna on 15/11/2020.
//

#include <utils/rendering_constants.h>
#include <utils/cassert.h>
#include "BoxModelBuilder.h"
#include "BoxNode.h"
#include "debugmalloc.h"
#include "AmfTextWidth.h"

struct BoxModelBuilder
{
	Logger *logger;
	ExpNode *exp_tree_root;
	AmfTextWidth amf_text_width;
};

BoxModelBuilder *BoxModelBuilder_new(Logger *logger, ExpNode *exp_tree_root)
{
	BoxModelBuilder *self = (BoxModelBuilder *) malloc(sizeof(*self));
	self->logger = logger;
	self->exp_tree_root = exp_tree_root;
	AmfTextWidth_init(&self->amf_text_width);

	return self;
}

void BoxModelBuilder_free(BoxModelBuilder *self)
{
	free(self);
}

BoxNode *BoxModelBuilder_build_box_for_node(BoxModelBuilder *self, ExpNode *exp_node);

void BoxModelBuilder_build_index_box(BoxModelBuilder *self, BoxNode *box_node)
{
	cassert(self->logger, box_node->node->arg1 != NULL, "Index needs a first argument");
	const Vector *delta = &SCRIPT_BOX_DELTA;
	Vector neg_delta;
	Vector_negate(delta, &neg_delta);
	double arg2_width = 0;
	double arg3_width = 0;

	if (box_node->node->arg1)
		box_node->arg1_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg1);

	if (box_node->node->arg2)
		box_node->arg2_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg2);

	if (box_node->node->arg3)
		box_node->arg3_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg3);


	double arg_offset = box_node->arg1_box->box.width + delta->x;
	if (box_node->arg2_box) {
		box_node->box.height += box_node->arg2_box->box.height + neg_delta.y;
		box_node->arg1_box->offset.y = box_node->arg2_box->box.height + neg_delta.y;
		box_node->arg2_box->offset.x = arg_offset;
		arg2_width = box_node->arg2_box->box.width;
	}
	if (box_node->arg3_box) {
		box_node->box.height += box_node->arg3_box->box.height + neg_delta.y;
		box_node->arg3_box->offset.x = arg_offset;
		box_node->arg3_box->offset.y = box_node->arg1_box->offset.y + box_node->arg1_box->box.height + neg_delta.y;
		arg3_width = box_node->arg3_box->box.width;
	}
	box_node->box.width = arg_offset + double_max(arg2_width, arg3_width);
	box_node->box.height += box_node->arg1_box->box.height;
}

void BoxModelBuilder_build_node_list_box(BoxModelBuilder *self, BoxNode *box_node)
{
	box_node->node_list_box = List_new();
	const double gap = NODE_LIST_GAP;

	for (size_t ii = 0; ii < box_node->node->node_list->item_count; ++ii) {
		ExpNode *at_node = List_get(box_node->node->node_list, ii);
		BoxNode *at_box = BoxModelBuilder_build_box_for_node(self, at_node);

		if (box_node->box.height < at_box->box.height)
			box_node->box.height = at_box->box.height;

		at_box->offset.x = box_node->box.width;
		box_node->box.width += at_box->box.width + gap;

		List_push(box_node->node_list_box, at_box);
	}

	for (size_t ii = 0; ii < box_node->node_list_box->item_count; ++ii) {
		BoxNode *at_box = List_get(box_node->node_list_box, ii);

		if (box_node->node->parent && box_node->node->parent->type == Frac) {
			if (box_node->node->parent->arg1 == box_node->node)
				Box_align_to_base_line(&at_box->offset, &at_box->box, &box_node->box);
			else
				at_box->offset.y = 0;
		} else
			Box_vertical_center(&at_box->offset, &at_box->box, &box_node->box);
	}

	box_node->box.width -= gap;
}

void BoxModelBuilder_build_frac_box(BoxModelBuilder *self, BoxNode *box_node)
{
	const double line_height = FRAC_LINE_HEIGHT;

	if (box_node->node->arg1) {
		box_node->arg1_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg1);
		box_node->box.height += box_node->arg1_box->box.height;
	}

	if (box_node->node->arg2) {
		box_node->arg2_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg2);
		box_node->box.height += box_node->arg2_box->box.height;
	}

	box_node->box.width = double_max(box_node->arg1_box ? box_node->arg1_box->box.width : 0,
									 box_node->arg2_box ? box_node->arg2_box->box.width : 0);

	box_node->box.height += line_height;

	if (box_node->arg1_box) {
		Box_horizontal_center(&box_node->arg1_box->offset, &box_node->arg1_box->box, &box_node->box);
	}
	if (box_node->arg2_box) {
		Box_horizontal_center(&box_node->arg2_box->offset, &box_node->arg2_box->box, &box_node->box);
		box_node->arg2_box->offset.y = (box_node->arg1_box ? box_node->arg1_box->box.height : 0) + line_height;
	}
}

void BoxModelBuilder_build_box_for_sqrt(BoxModelBuilder *self, BoxNode *box_node)
{
	const Vector *delta = &SQRT_BOX_DELTA;

	if (box_node->node->arg1) {
		// TODO unimplemented
		cassert(self->logger, false, "Sqrt with args not supported yet.");
	} else {
		box_node->arg1_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg1);
		Size_add_s(&box_node->box, &box_node->arg1_box->box);
		Size_add_v(&box_node->box, delta);
		Vector_add_v(&box_node->arg1_box->offset, delta);
	}
}

void BoxModelBuilder_build_prod_sum_box(BoxModelBuilder *self, BoxNode *box_node)
{
	const Size *sum_prod_size = box_node->node->type == Sum ? &SUM_SIZE : &PROD_SIZE;
	BoxModelBuilder_build_frac_box(self, box_node);
	box_node->box.height = sum_prod_size->height + SUM_PROD_PADDING * 2;
	box_node->box.width = double_max(box_node->box.width, sum_prod_size->width);

	if (box_node->arg1_box)
		box_node->box.height += box_node->arg1_box->box.height;

	if (box_node->arg2_box) {
		box_node->arg2_box->offset.y = SUM_PROD_PADDING * 2 + sum_prod_size->height +
									   (box_node->arg1_box ? box_node->arg1_box->box.height : 0);
		box_node->box.height += box_node->arg2_box->box.height;
	}
}

void BoxModelBuilder_build_lim_box(BoxModelBuilder *self, BoxNode *box_node)
{
	box_node->box.height = LIM_SIZE.height;
	if (box_node->node->arg1)
	{
		box_node->arg1_box = BoxModelBuilder_build_box_for_node(self, box_node->node->arg1);
		box_node->box.height += box_node->arg1_box->box.height + LIM_PADDING;
		box_node->box.width = double_max(LIM_SIZE.width, box_node->arg1_box->box.width);
		box_node->arg1_box->offset.y += LIM_SIZE.height + LIM_PADDING;
		Box_horizontal_center(&box_node->arg1_box->offset, &box_node->arg1_box->box, &box_node->box);
	}
}

void BoxModelBuilder_build_literal_box(BoxModelBuilder *self, BoxNode *box_node)
{
	box_node->box.height = TEXT_HEIGHT + TEXT_CORRECTION;
	box_node->box.width = 0;
	const int length = DString_len(box_node->node->value);

	for (int ii = 0; ii < length; ++ii) {
		box_node->box.width += AmfTextWidth_char_width_calc(&self->amf_text_width,
															DString_char_at(box_node->node->value, ii), TEXT_HEIGHT);
	}
	box_node->box.width += length * LETTER_SPACING;
}

BoxNode *BoxModelBuilder_build_box_for_node(BoxModelBuilder *self, ExpNode *exp_node)
{
	BoxNode *box = BoxNode_new(exp_node);

	if (exp_node->type == Symbol) {
		box->box.width = TEXT_WIDTH;
		box->box.height = TEXT_HEIGHT + TEXT_CORRECTION;
	} else if (exp_node->type == Literal) {
		BoxModelBuilder_build_literal_box(self, box);
	} else if (exp_node->type == Index) {
		BoxModelBuilder_build_index_box(self, box);
	} else if (exp_node->type == NodeList) {
		BoxModelBuilder_build_node_list_box(self, box);
	} else if (exp_node->type == Frac) {
		BoxModelBuilder_build_frac_box(self, box);
	} else if (exp_node->type == Sqrt) {
		BoxModelBuilder_build_box_for_sqrt(self, box);
	} else if (exp_node->type == Sum || exp_node->type == Prod) {
		BoxModelBuilder_build_prod_sum_box(self, box);
	} else if (exp_node->type == Lim) {
		BoxModelBuilder_build_lim_box(self, box);
	}

	return box;
}

BoxNode *BoxModelBuilder_build(BoxModelBuilder *self)
{
	Logger_log(self->logger, LogInfo, "STEP 3. Building box model.");

	BoxNode *root = BoxModelBuilder_build_box_for_node(self, self->exp_tree_root);
	return root;
}
