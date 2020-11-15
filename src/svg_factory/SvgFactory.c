#include <expression_parser/ExpNode.h>
#include <logger/logger.h>
#include <svg_file/SvgFile.h>
#include <utils/cassert.h>
#include <assert.h>
#include <utils/geomerty.h>
#include <utils/rendering_constants.h>
#include "SvgFactory.h"
#include "debugmalloc.h"
#include "BoxNode.h"

struct SvgFactory
{
	Logger *logger;
	ExpNode *exp_tree_root;
	BoxNode *box_tree_root;

	char *out_file_path;

	SvgFile *svg_file;
};

SvgFactory *SvgFactory_new(Logger *logger, ExpNode *exp_tree_root, char *out_file_path)
{
	SvgFactory *self = (SvgFactory *) malloc(sizeof(*self));
	assert(self && "Couldn't allocate SvgFactory");

	self->logger = logger;
	self->exp_tree_root = exp_tree_root;
	self->box_tree_root = NULL;
	self->out_file_path = out_file_path;

	FILE *file = fopen(self->out_file_path, "w");
	cassert(self->logger, !!file, "Couldn't open output file to write to.");

	self->svg_file = SvgFile_new(file);

	return self;
}

BoxNode *SvgFactory_build_box_for_node(ExpNode *exp_node);

void SvgFactory_build_script_box(BoxNode *box_node, bool is_super)
{
	const Vector *delta = &SCRIPT_BOX_DELTA;

	if (box_node->node->arg1)
		box_node->arg1_box = SvgFactory_build_box_for_node(box_node->node->arg1);

	if (box_node->node->arg2)
		box_node->arg2_box = SvgFactory_build_box_for_node(box_node->node->arg2);

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

void SvgFactory_build_node_list_box(BoxNode *box_node)
{
	box_node->node_list_box = List_new();
	const double gap = NODE_LIST_GAP;

	for (size_t ii = 0; ii < box_node->node->node_list->item_count; ++ii) {
		ExpNode *at_node = List_get(box_node->node->node_list, ii);
		BoxNode *at_box = SvgFactory_build_box_for_node(at_node);

		if (box_node->box.height < at_box->box.height)
			box_node->box.height = at_box->box.height;

		at_box->offset.x = box_node->box.with;
		box_node->box.with += at_box->box.with + gap;

		List_push(box_node->node_list_box, at_box);
	}
	box_node->box.with -= gap;
}

void SvgFactory_build_frac_box(BoxNode *box_node)
{
	const double line_height = FRAC_LINE_HEIGHT;

	if (box_node->node->arg1) {
		box_node->arg1_box = SvgFactory_build_box_for_node(box_node->node->arg1);
		box_node->box.height += box_node->arg1_box->box.height;
	}

	if (box_node->node->arg2) {
		box_node->arg2_box = SvgFactory_build_box_for_node(box_node->node->arg2);
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

void SvgFactory_build_box_for_sqrt(BoxNode *box_node)
{
	const Vector *delta = &SQRT_BOX_DELTA;

	box_node->arg1_box = SvgFactory_build_box_for_node(box_node->node->arg1);
	Size_add_s(&box_node->box, &box_node->arg1_box->box);
	Size_add_v(&box_node->box, delta);
}

void SvgFactory_build_prod_sum_box(BoxNode *box_node)
{
	SvgFactory_build_frac_box(box_node);
	box_node->box.height -= FRAC_LINE_HEIGHT;
	box_node->box.height += SUM_PROD_SIZE.height;
	box_node->box.with = double_max(box_node->box.with, SUM_PROD_SIZE.with);
}

BoxNode *SvgFactory_build_box_for_node(ExpNode *exp_node)
{
	BoxNode *box = BoxNode_new(exp_node);

	if (exp_node->type == Symbol) {
		box->box.with = 20;
		box->box.height = 20;
	} else if (exp_node->type == Literal) {
		box->box.height = 15;
		box->box.with = DString_len(exp_node->value) * 10;
	} else if (exp_node->type == SuperScript) {
		SvgFactory_build_script_box(box, true);
	} else if (exp_node->type == SubScript) {
		SvgFactory_build_script_box(box, false);
	} else if (exp_node->type == NodeList) {
		SvgFactory_build_node_list_box(box);
	} else if (exp_node->type == Frac) {
		SvgFactory_build_frac_box(box);
	} else if (exp_node->type == Sqrt) {
		SvgFactory_build_box_for_sqrt(box);
	} else if (exp_node->type == Sum || exp_node->type == Prod) {
		SvgFactory_build_prod_sum_box(box);
	}

	return box;
}

void SvgFactory_build_box_model(SvgFactory *self)
{
	Logger_log(self->logger, LogInfo, "STEP 3. Building box model.");

	BoxNode *root = SvgFactory_build_box_for_node(self->exp_tree_root);
	self->box_tree_root = root;
}

void SvgFactory_create(SvgFactory *self)
{
	Logger_log(self->logger, LogInfo, "box stuff");
	SvgFactory_build_box_model(self);
}

void SvgFactory_free(SvgFactory *self)
{
	if (self->box_tree_root)
		BoxNode_free(self->box_tree_root);

	SvgFile_close(self->svg_file);
	free(self);
}
